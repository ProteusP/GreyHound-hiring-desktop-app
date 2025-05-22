#pragma once

#include "drogon/HttpTypes.h"
#include "filters/api_v1_checkAuthFilter.h"
#include <drogon/HttpController.h>
using namespace drogon;

namespace api::v1 {
    class profile final : public drogon::HttpController<profile> {
        static void getProfile(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback);

        static void patchProfile(const HttpRequestPtr &req,
                                 std::function<void(const HttpResponsePtr &)> &&callback);

    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(profile::getProfile, "/", drogon::Get,
                       "api::v1::checkAuthFilter"); // path is /api/v1/profile
            METHOD_ADD(profile::patchProfile, "/", drogon::Patch,
                       "api::v1::checkAuthFilter"); // path is /api/v1/profile
        METHOD_LIST_END
    };
}

void saveUserProfile(const std::string& id, const Json::Value& data);