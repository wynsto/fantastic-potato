#include <doctest/doctest.h>
#include <fantastic_potato/fantastic_potato.h>
#include <fantastic_potato/version.h>

#include <string>

TEST_CASE("FantasticPotato") {
  using namespace fantastic_potato;

  FantasticPotato fantastic_potato("Tests");

  CHECK(fantastic_potato.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(fantastic_potato.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(fantastic_potato.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(fantastic_potato.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("FantasticPotato version") {
  static_assert(std::string_view(GREETER_VERSION) == std::string_view("1.0"));
  CHECK(std::string(GREETER_VERSION) == std::string("1.0"));
}
