#include "api_v1_resources.h"

#include <exception>
#include <fstream>
#include <string>

#include "Candidates.h"
#include "Vacancies.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/orm/Criteria.h"
#include "drogon/orm/Mapper.h"
#include "trantor/utils/Logger.h"

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

void resources::getCandidateCards(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

  //starts from 1
  int page = 1;
  int pageSize = 20;

  const auto& pageStr = req->getParameter("page");
  const auto& pageSizeStr = req->getParameter("pageSize");

  if (!pageStr.empty()) page =std::stoi(pageStr);
  if (!pageSizeStr.empty()) pageSize = std::stoi(pageSizeStr);

  if (page < 1) page = 1;
  if (pageSize < 1)  pageSize = 1;
  if (pageSize > 20) pageSize = 20;

  const int offset = (page - 1) * pageSize;
  // not noexcept!!!
  const auto& dbClient = app().getDbClient();

  // TODO
  // No position in our DB now
  dbClient ->execSqlAsync(
    "SELECT user_id, place_of_study, faculty_of_educ, experience_status_id FROM candidates LIMIT ? OFFSET ?",
    [callback, dbClient, page, pageSize](const drogon::orm::Result& candidatesResult) mutable {

      std::vector<int> candidateIds;
      for (const auto& row : candidatesResult) {
        candidateIds.push_back(row["user_id"].as<int>());
      }

      if (candidateIds.empty()) {
        Json::Value jsonResp;
        jsonResp["candidates"] = Json::arrayValue;
        jsonResp["page"] = page;
        jsonResp["pageSize"] = pageSize;
        jsonResp["count"] = 0;

        const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
        callback(resp);
        return;
      }
        std::string placeholders;

      for (size_t i = 0; i < candidateIds.size() -1; ++i) {
        placeholders.append(std::to_string(candidateIds[i]) + ",");
      }
      placeholders.append(std::to_string(candidateIds.back()));


      const std::string sql = "SELECT cs.user_id, exp.name "
                        "FROM candidates cs "
                        "JOIN experience exp ON cs.experience_status_id = exp.id "
                        "WHERE cs.user_id IN (" + placeholders + ")";

      dbClient->execSqlAsync(
        sql,
        [callback, candidatesResult, candidateIds, page, pageSize](const drogon::orm::Result& expResult) mutable {
          std::unordered_map<int, std::string> candidateExp;
          for (const auto& row : expResult) {
            int cid = row["user_id"].as<int>();
            const auto exp = row["name"].as<std::string>();

            candidateExp[cid] = exp;
          }

          Json::Value jsonResp;
          Json::Value candidatesJson(Json::arrayValue);

          for (const auto& row : candidatesResult) {
            const int cid = row["user_id"].as<int>();
            Json::Value candidateJson;
            candidateJson["id"] = cid;
            const auto placeOfStudy = row["place_of_study"].as<std::string>();
            const auto facultyOfEducation = row["faculty_of_educ"].as<std::string>();
            candidateJson["place"] = placeOfStudy;
            candidateJson["faculty"] = facultyOfEducation;
            candidateJson["experience"] = candidateExp[cid];
            candidateJson["user_id"] = cid;
            candidatesJson.append(candidateJson);
          }

          jsonResp["candidates"] = candidatesJson;
          jsonResp["page"] = page;
          jsonResp["pageSize"] = pageSize;
          jsonResp["count"] = static_cast<int>(candidateIds.size());

          const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
          callback(resp);
        }
        ,[callback](const drogon::orm::DrogonDbException& e) {
          Json::Value jsonResp;
          jsonResp["error"] = "Database error on experience query";
          const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
          resp->setStatusCode(HttpStatusCode::k500InternalServerError);
          callback(resp);
        });

    },
    [callback](const drogon::orm::DrogonDbException& e) {
      Json::Value jsonResp;
      jsonResp["error"] = "Database error on query";
      const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(HttpStatusCode::k500InternalServerError);
      callback(resp);
    },
    pageSize, offset);
}

void resources::createVacancy(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback){

        const auto& session = req->getSession();
        const auto &userId = session->get<std::string>("user_id");

        auto jsonData = req->getJsonObject();
        LOG_DEBUG << jsonData->toStyledString();

        const auto& dbClient = app().getDbClient();

        int vacId;
        auto vacMapper = orm::Mapper<drogon_model::default_db::Vacancies>(dbClient);

        Json::Value jsonResp;
        try{
        drogon_model::default_db::Vacancies vacancy(*jsonData);

        vacancy.setEmployerId(std::stoi(userId));

        vacMapper.insert(vacancy);
        vacId = *vacancy.getId();

        }catch (const std::exception& err){
            LOG_DEBUG << "Error while parsing JSON to vacancy: "<<err.what()<<'\n';

            jsonResp["error"] = "JSON format error";
            const auto& resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
            return;
        }


        jsonResp["vacancy_id"] = vacId;
        const auto& resp = HttpResponse::newHttpJsonResponse(jsonResp);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    }

    //TODO: make this ASYNC
    void resources::deleteVacancy(
        const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const int vacId
    ){
        const auto& dbClient = app().getDbClient();
        auto vacMapper = orm::Mapper<drogon_model::default_db::Vacancies>(dbClient);
        const std::string& vacIdStr = std::to_string(vacId);

        const int countOfDeleted = vacMapper.deleteBy(orm::Criteria(drogon_model::default_db::Vacancies::Cols::_id, orm::CompareOperator::EQ, vacIdStr));

        Json::Value jsonResp;
        if (countOfDeleted != 0){
            jsonResp["count"] = countOfDeleted;

            const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }else{
            jsonResp["error"] = "no vacancies with this ID";

            const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(drogon::k400BadRequest);
            callback(resp);
        }

    }

    void resources::getEmplVacancies(
        const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback
    ){
        const auto& session = req->getSession();
        const auto &userId = session->get<std::string>("user_id");
        const auto& dbClient = app().getDbClient();

        auto vacCriteria = orm::Criteria(drogon_model::default_db::Vacancies::Cols::_employer_id, orm::CompareOperator::EQ, userId);

        auto vacMapper = orm::Mapper<drogon_model::default_db::Vacancies>(dbClient);

        const auto& vacs = vacMapper.findBy(vacCriteria);

        Json::Value vacsJson(Json::arrayValue);

        Json::Value jsonResp;

        for (const auto& vac : vacs) {
            vacsJson.append(vac.toJson());
        }

        const int count = vacs.size();
        jsonResp["vacancies"] = vacsJson;
        jsonResp["count"] = count;


        const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
        resp->setStatusCode(HttpStatusCode::k200OK);
        callback(resp);
    }
