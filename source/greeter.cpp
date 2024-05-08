#include <fmt/format.h>
#include <greeter/greeter.h>

using namespace fantastic_potato;

Potato::Potato(std::string _name) : name(std::move(_name)) {}

std::string Potato::greet(LanguageCode lang) const {
  switch (lang) {
    default:
    case LanguageCode::EN:
      return fmt::format("Hello, Potato {}!", name);
    case LanguageCode::DE:
      return fmt::format("Hallo Potato {}!", name);
    case LanguageCode::ES:
      return fmt::format("¡Hola Potato {}!", name);
    case LanguageCode::FR:
      return fmt::format("Bonjour Potato {}!", name);
  }
}
