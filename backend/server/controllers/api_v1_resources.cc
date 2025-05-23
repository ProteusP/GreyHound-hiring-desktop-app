#include "api_v1_resources.h"
#include "Candidates.h"
#include "Vacancies.h"
#include "drogon/orm/Mapper.h"

using namespace api::v1;

// Add definition of your processing function here
void resources::getResumes(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  Json::Value jsonResp;
  try {
    const auto &dbClient = app().getDbClient();
    auto mapper = orm::Mapper<drogon_model::default_db::Candidates>(dbClient);

    const auto &candidates = mapper.findAll();

    if (candidates.empty()) {
      jsonResp["message"] = "No candidates found";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

      resp->setStatusCode(HttpStatusCode::k404NotFound);
      callback(resp);
      return;
    }

    for (const auto &candidate : candidates) {
      jsonResp["data"].append(candidate.toJson());
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(HttpStatusCode::k200OK);
    callback(resp);
  } catch (...) {
    jsonResp["message"] = "Internal Server Error";
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

    resp->setStatusCode(HttpStatusCode::k500InternalServerError);
    callback(resp);
  }
};

void resources::getVacancies(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  Json::Value jsonResp;

  try {
    const auto &dbClient = app().getDbClient();
    auto mapper = orm::Mapper<drogon_model::default_db::Vacancies>(dbClient);

    const auto &vacancies = mapper.findAll();

    if (vacancies.empty()) {
      jsonResp["message"] = "No vacancies found";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

      resp->setStatusCode(HttpStatusCode::k404NotFound);
      callback(resp);
      return;
    }

    for (const auto &vacancy : vacancies) {
      jsonResp["data"].append(vacancy.toJson());
    }
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

    resp->setStatusCode(HttpStatusCode::k200OK);
    callback(resp);
  } catch (...) {
    jsonResp["message"] = "Internal Server Error";
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

    resp->setStatusCode(HttpStatusCode::k500InternalServerError);
    callback(resp);
  }
}
