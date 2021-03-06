#include "launcher.hh"
#include <cassert>
#include <exec/command.hh>

namespace ast
{
namespace visitors
{
struct Launcher::Implem
{
  exec::Command cmd{};
  exec::Command* current_cmd = &cmd;
};

Launcher::Launcher()
    : VisitorInterface::VisitorInterface()
    , pimpl_{ std::make_unique<Implem>() }
{
}

Launcher::~Launcher() noexcept = default;

void Launcher::operator()(const std::string& str)
{
  *pimpl_->current_cmd += str;
}

void Launcher::operator()(const ast::redir_node&)
{
}

void Launcher::operator()(const ast::cmd_element& element)
{
  element.apply_visitor(*this);
}

void Launcher::operator()(const ast::cmd_node& node)
{
  for (const auto& element : node)
    operator()(element);
  assert(pimpl_->cmd.executable());
}

void Launcher::operator()(const ast::operand& op)
{
  op.apply_visitor(*this);
}

void Launcher::operator()(const ast::operator_node& node)
{
  using parser::grammar::symbol_type;

  assert(pimpl_->cmd.executable());

  switch (node.op_type)
  {
  case symbol_type::DOUBLE_AND:
    pimpl_->current_cmd = &pimpl_->cmd;
    if (pimpl_->cmd())
      return;
    break;
  case symbol_type::DOUBLE_OR:
    pimpl_->current_cmd = &pimpl_->cmd;
    if (not pimpl_->cmd())
      return;
    break;
  case symbol_type::OR:
  pimpl_->current_cmd = pimpl_->current_cmd->push_to_pipeline();
  break;
  default:
    assert(false && "invalid operation");
    break;
  }
  operator()(node.second);
}

void Launcher::operator()(const ast::expression_node& node)
{
  operator()(node.first);
  for (const auto& operation : node.rest)
    operator()(operation);
}

void Launcher::operator()(const ast::statement_node& node)
{
  operator()(node.exp);
  if (pimpl_->cmd.executable()) // then is has not been executed. So, execute it
  {
    pimpl_->current_cmd = &pimpl_->cmd;
    pimpl_->cmd();
  }
}

void Launcher::operator()(const ast::ast_root& node)
{
  if (node)
    for (const auto& statement : *node)
      operator()(statement);
}

} // visitors
} // ast
