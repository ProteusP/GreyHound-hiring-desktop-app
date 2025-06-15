#pragma once

#include <drogon/HttpController.h>
#include <drogon/HttpTypes.h>

using namespace drogon;

namespace api::v1 {

const std::string EMPL_STATUS = "empl";
const std::string CAND_STATUS = "candidate";
const std::string USERS_TABLE = "users";

class Auth final : public drogon::HttpController<Auth> {
    static void login(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback);

    static void
    registerUser(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback);

    static void logout(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback);

  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Auth::login, "/login",
               drogon::Post); // path is /api/v1/Auth/login
    METHOD_ADD(Auth::registerUser, "/register",
               drogon::Post); // path is /api/v1/Auth/register

    METHOD_ADD(Auth::logout, "logout",
               drogon::Get); // path is /api/v1/Auth/logout
    METHOD_LIST_END
};
} // namespace api::v1
