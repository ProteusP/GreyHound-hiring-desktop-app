#pragma once

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpTypes.h>

using namespace drogon;

namespace api {
namespace v1 {

const std::string EMPL_STATUS = "empl";
const std::string CAND_STATUS = "candidate";

class Auth : public drogon::HttpController<Auth> {
  void login(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback);

public:
  METHOD_LIST_BEGIN
  // use METHOD_ADD to add your custom processing function here;
  // METHOD_ADD(Auth::get, "/{2}/{1}", Get); // path is
  // /api/v1/Auth/{arg2}/{arg1} METHOD_ADD(Auth::your_method_name,
  // "/{1}/{2}/list", Get); // path is /api/v1/Auth/{arg1}/{arg2}/list
  // ADD_METHOD_TO(Auth::your_method_name, "/absolute/path/{1}/{2}/list", Get);
  // // path is /absolute/path/{arg1}/{arg2}/list
  METHOD_ADD(Auth::login, "/login", drogon::Post);
  METHOD_LIST_END
  // your declaration of processing function maybe like this:
  // void get(const HttpRequestPtr& req, std::function<void (const
  // HttpResponsePtr &)> &&callback, int p1, std::string p2); void
  // your_method_name(const HttpRequestPtr& req, std::function<void (const
  // HttpResponsePtr &)> &&callback, double p1, int p2) const;
};
} // namespace v1
} // namespace api
