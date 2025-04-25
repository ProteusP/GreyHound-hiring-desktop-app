#pragma once

#include "drogon/HttpTypes.h"
#include "filters/api_v1_checkAuthFilter.h"
#include <drogon/HttpController.h>
using namespace drogon;

namespace api {
namespace v1 {
class profile : public drogon::HttpController<profile> {
  void getProfile(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback);
  void patchProfile(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback);

public:
  METHOD_LIST_BEGIN
  METHOD_ADD(profile::getProfile, "/", drogon::Get,
             "api::v1::checkAuthFilter"); // path is /api/v1/profile
  METHOD_ADD(profile::patchProfile, "/", drogon::Patch,
             "api::v1::checkAuthFilter"); // path is /api/v1/profile
  METHOD_LIST_END
};
} // namespace v1
} // namespace api
