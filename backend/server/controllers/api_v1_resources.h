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
            std::function<void(const HttpResponsePtr &)> &&callback,
            const std::string& userId);

    static void saveResume(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback);
static void getCandidateCards(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

public:
  METHOD_LIST_BEGIN
  METHOD_ADD(resources::getResumes, "/resumes", drogon::Get,
             "api::v1::checkAuthFilter"); // path is /api/v1/resources/resumes
  METHOD_ADD(resources::getVacancies, "/vacancies", drogon::Get,
             "api::v1::checkAuthFilter"); // path is /api/v1/resources/vacancies
    METHOD_ADD(resources::getResume, "/resume/{1}", drogon::Get /* "api::v1::checkAuthFilter" */); // path is /api/v1/resources/resume/{user_id}

    METHOD_ADD(resources::saveResume, "resume", drogon::Post, "api::v1::checkAuthFilter"); // path is /api/v1/resources/resume

    METHOD_ADD(resources::getCandidateCards, "/candidateCards", drogon::Get); // path is /api/v1/resources/candidateCards/?page=..&pageSize=..
  METHOD_LIST_END
};
} // namespace api::v1
