#pragma once

#include "filters/api_v1_checkAuthFilter.h"
#include <drogon/HttpController.h>

using namespace drogon;

namespace api::v1 {
class resources final : public drogon::HttpController<resources> {
  static void
  getResumes(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback);

  static void
  getVacancies(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback);

  static void
  getResume(const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback);

public:
  METHOD_LIST_BEGIN
  METHOD_ADD(resources::getResumes, "/resumes", drogon::Get,
             "api::v1::checkAuthFilter"); // path is /api/v1/resources/resumes
  METHOD_ADD(resources::getVacancies, "/vacancies", drogon::Get,
             "api::v1::checkAuthFilter"); // path is /api/v1/resources/vacancies
  METHOD_LIST_END
};
} // namespace api::v1
