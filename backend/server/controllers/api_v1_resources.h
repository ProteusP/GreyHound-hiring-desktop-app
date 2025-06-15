#pragma once

#include "drogon/HttpTypes.h"
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


static void createVacancy(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

static void deleteVacancy(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const int vacId
);

static void getEmplVacancies(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback
);

static void updateVacancy(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const int vacId
);

static void getVacanciesCards(const
    HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback
);

static void getCandidateInfo(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string userId);


public:
    METHOD_LIST_BEGIN
    METHOD_ADD(resources::getResumes, "/resumes", drogon::Get,
                "api::v1::checkAuthFilter"); // path is /api/v1/resources/resumes
    METHOD_ADD(resources::getVacancies, "/vacancies", drogon::Get,
                "api::v1::checkAuthFilter"); // path is /api/v1/resources/vacancies
    METHOD_ADD(resources::getResume, "/resume/{1}", drogon::Get /* "api::v1::checkAuthFilter" */); // path is /api/v1/resources/resume/{user_id}

    METHOD_ADD(resources::saveResume, "resume", drogon::Post, "api::v1::checkAuthFilter"); // path is /api/v1/resources/resume

    METHOD_ADD(resources::getCandidateCards, "/candidateCards", drogon::Get); // path is /api/v1/resources/candidateCards/?page=..&pageSize=..

    // no auth filter for faster responses
    METHOD_ADD(resources::createVacancy, "createVacancy", drogon::Post, "api::v1::checkAuthFilter"); // path is /api/v1/resources/createVacancy

    METHOD_ADD(resources::deleteVacancy, "deleteVacancy/{1}", drogon::Delete, "api::v1::checkAuthFilter"); // path is /api/v1/resources/deleteVacancy/{vacancy_id}

    METHOD_ADD(resources::getEmplVacancies, "emplVacancies", drogon::Get, "api::v1::checkAuthFilter"); // path is /api/v1/resources/emplVacancies

    METHOD_ADD(resources::updateVacancy, "updateVacancy/{1}", drogon::Patch, "api::v1::checkAuthFilter"); // path is /api/v1/resources/updateVacancy/{vacancy_id}

    METHOD_ADD(resources::getVacanciesCards, "/vacanciesCards", drogon::Get); // path is /api/v1/resources/vacanciesCards/?page=..&pageSize=..
    // no auth filter for faster responses

    METHOD_ADD(resources::getCandidateInfo, "/candidateInfo/{1}", drogon::Get); // path is /api/v1/resources/candidateInfo/{id}

    METHOD_LIST_END
};
} // namespace api::v1
