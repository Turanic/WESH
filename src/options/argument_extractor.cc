#include "argument_extractor.hh"
#include <memory>

namespace options
{
ArgumentExtractor::ArgumentExtractor(std::string argument) noexcept
  : input_{ std::move(argument) }
{
}

std::string ArgumentExtractor::input_get()
{
  return input_ + '\n';
}

bool ArgumentExtractor::should_loop() noexcept
{
  return false;
}
} // options
