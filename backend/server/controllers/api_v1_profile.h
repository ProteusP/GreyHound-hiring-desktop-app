#pragma once

#include "drogon/HttpTypes.h"
#include <drogon/HttpController.h>
#include "filters/api_v1_checkAuthFilter.h"
using namespace drogon;

namespace api
{
namespace v1
{
class profile : public drogon::HttpController<profile>
{
    void getProfile(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(profile::get, "/{2}/{1}", Get); // path is /api/v1/profile/{arg2}/{arg1}
    // METHOD_ADD(profile::your_method_name, "/{1}/{2}/list", Get); // path is /api/v1/profile/{arg1}/{arg2}/list
    // ADD_METHOD_TO(profile::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(profile::getProfile, "/", drogon::Get, "api::v1::checkAuthFilter");
    METHOD_LIST_END

};
}
}
