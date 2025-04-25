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
  void registerUser(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback);

public:
  METHOD_LIST_BEGIN
  METHOD_ADD(Auth::login, "/login",
             drogon::Post); // path is path is /api/v1/profile/login
  METHOD_ADD(Auth::registerUser, "/register",
             drogon::Post); // path is path is /api/v1/profile/register
  METHOD_LIST_END
};
} // namespace v1
} // namespace api
