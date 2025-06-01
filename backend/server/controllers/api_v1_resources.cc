#include "api_v1_resources.h"

#include <fstream>

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
      const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

      resp->setStatusCode(HttpStatusCode::k404NotFound);
      callback(resp);
      return;
    }

    for (const auto &candidate : candidates) {
      jsonResp["data"].append(candidate.toJson());
    }
    const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(HttpStatusCode::k200OK);
    callback(resp);
  } catch (...) {
    jsonResp["message"] = "Internal Server Error";
    const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

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
      const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

      resp->setStatusCode(HttpStatusCode::k404NotFound);
      callback(resp);
      return;
    }

    for (const auto &vacancy : vacancies) {
      jsonResp["data"].append(vacancy.toJson());
    }
    const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

    resp->setStatusCode(HttpStatusCode::k200OK);
    callback(resp);
  } catch (...) {
    jsonResp["message"] = "Internal Server Error";
    const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

    resp->setStatusCode(HttpStatusCode::k500InternalServerError);
    callback(resp);
  }
}

std::string makeResumePath(const std::string &userId) {
  return "data/" + userId + "_resume.pdf";
}

void resources::getResume(const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback,
            const std::string& userId) {


  const std::string filePath = app().getUploadPath() + "/" + makeResumePath(userId);
  LOG_DEBUG << "Resume path: " << filePath;
  std::ifstream file(filePath,std::ios::binary);

  if (!file.is_open()) {
    const auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(HttpStatusCode::k404NotFound);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("Resume file not found");
    callback(resp);
    return;
  }

  std::ostringstream ss;
  ss << file.rdbuf();
  std::string content = ss.str();

  auto resp = HttpResponse::newHttpResponse();
  resp->setContentTypeCode(CT_APPLICATION_PDF);
  resp->addHeader("Content-Disposition", "inline; filename=\"" + userId + "_resume.pdf\"");
  resp->setBody(content);
  callback(resp);

}

void resources::saveResume(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
  const auto& session = req->getSession();
  const auto &userId = session->get<std::string>("user_id");

  MultiPartParser fileParser;
  fileParser.parse(req);

  if (fileParser.getFiles().empty()) {
    const auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(HttpStatusCode::k400BadRequest);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("Not Found");
    callback(resp);
    return;
  }

  if (const auto& ext = fileParser.getFiles()[0].getFileExtension(); ext != "pdf") {
    const auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(HttpStatusCode::k400BadRequest);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("Resume should be PDF file");
    callback(resp);
    return;
  }

  const auto filePath = makeResumePath(userId);

  if (const auto res = fileParser.getFiles()[0].saveAs(filePath); res != 0) {
    const auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(HttpStatusCode::k500InternalServerError);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("Resume not saved");
    callback(resp);
    return;
  }

  const auto resp = HttpResponse::newHttpResponse();
  resp->setStatusCode(HttpStatusCode::k200OK);
  resp->setContentTypeCode(CT_TEXT_PLAIN);
  resp->setBody("Resume saved!");
  callback(resp);
}