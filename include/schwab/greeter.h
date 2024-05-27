#pragma once

#include <string>

using namespace std;


namespace schwab_personal_trading {

  /**
   * @brief A class for saying hello in multiple languages
   */
  class Potato {
    string name;
    string auth;
    string bearerAuth;

  public:
    /**
     * @brief Creates a new greeter
     * @param name the name to greet
     */
    Potato(string name);

    string post(string url, string data) const;
  
    string get(string url) const;

    string getAfterAuth(string url) const;

    string getAccessToken(string grant_type, string token);

    void setAuth(string auth);

    void setBearerAuth(string bearerAuth);

  };

}  // namespace greeter
