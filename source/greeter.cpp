#include <curl/curl.h>
#include <fmt/format.h>
#include <schwab/db.h>
#include <schwab/greeter.h>

#include <boost/beast/core/detail/base64.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace schwab_personal_trading;
using namespace std;
using namespace boost::beast::detail::base64;

using json = nlohmann::json;

string baseUrl = "";

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

Potato::Potato(string _name) : name(std::move(_name)) {
  string appKey = getenv("SCHWAB_APP_KEY");
  string appSecret = getenv("SCHWAB_APP_SECRET");

  string base64Code;
  string src = appKey + ":" + appSecret;
  base64Code.resize(encoded_size(src.size()));

  encode(&base64Code[0], src.c_str(), src.size());
  auth = "Authorization: Basic " + base64Code;
}

string Potato::post(string url, string data) const {
  CURL *curl;
  CURLcode res;
  string readBuffer;
  curl = curl_easy_init();

  struct curl_slist *headers = NULL; /* init to NULL is important */
  headers = curl_slist_append(headers, auth.c_str());
  headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "error";
}

string Potato::get(string url) const {
  CURL *curl;
  CURLcode res;
  string readBuffer;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    auto res = json::parse(readBuffer);
    if (!res.empty()) {
      return res[0]["date"];
    }
    return readBuffer;
  }
  return "error";
}

string Potato::getAccessToken(string grantType, string token) {
  string callbackUrl = getenv("SCHWAB_CALLBACK_URL");
  string baseUrl = "https://api.schwabapi.com/v1";
  string codeKey = grantType == "authorization_code" ? "code" : grantType;

  string postData = fmt::format("grant_type={}&{}={}&redirect_uri={}", grantType, codeKey, token,
                                callbackUrl);

  string tokenUrl = baseUrl + "/oauth/token";
  schwab_personal_trading::SchwabDB db("test.db");

  string resp = this->post(tokenUrl, postData);
  cout << resp << endl;
  auto data = json::parse(resp);
  string accessToken, refreshToken;
  if (data.contains("refresh_token") && data.contains("access_token")) {
    refreshToken = data["refresh_token"];
    accessToken = data["access_token"];
    cout << refreshToken << " and " << accessToken << endl;
    if (typeid(refreshToken) == typeid(string))
      db.query("insert or replace into schwab_kv values('refreshToken', '" + refreshToken + "')");
    if (typeid(accessToken) == typeid(string))
      db.query("insert or replace into schwab_kv values('accessToken', '" + accessToken + "')");
  }
  return accessToken;
}

string Potato::getAfterAuth(string url) const {
  CURL *curl;
  CURLcode res;
  string readBuffer;
  curl = curl_easy_init();

  struct curl_slist *headers = NULL; /* init to NULL is important */
  headers = curl_slist_append(headers, bearerAuth.c_str());
  headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return "error";
}

void Potato::setBearerAuth(string accessToken) {
  bearerAuth = "Authorization: Bearer " + accessToken;
}
