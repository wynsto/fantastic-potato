#pragma once

#include <string>

namespace fantastic_potato {

  /**  Language codes to be used with the Greeter class */
  enum class LanguageCode { EN, DE, ES, FR };

  /**
   * @brief A class for saying hello in multiple languages
   */
  class Potato {
    std::string name;
    std::string auth;
    std::string bearerAuth;

  public:
    /**
     * @brief Creates a new greeter
     * @param name the name to greet
     */
    Potato(std::string name);

    /**
     * @brief Creates a localized string containing the greeting
     * @param lang the language to greet in
     * @return a string containing the greeting
     */
    std::string greet(LanguageCode lang = LanguageCode::EN) const;

    std::string post(std::string url, std::string data) const;
  
    std::string get(std::string url) const;

    std::string getAfterAuth(std::string url) const;

    void setAuth(std::string auth);

    void setBearerAuth(std::string bearerAuth);
  };

}  // namespace greeter
