#include "api_v1_resources.h"

#include <exception>
#include <fstream>
#include <string>
#include <unordered_map>

#include "Candidates.h"
#include "Vacancies.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/orm/Criteria.h"
#include "drogon/orm/DbClient.h"
#include "drogon/orm/Exception.h"
#include "drogon/orm/Mapper.h"
#include "drogon/orm/Result.h"
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

  int page = 1;
  int pageSize = 20;

  const auto &pageStr = req->getParameter("page");
  const auto &pageSizeStr = req->getParameter("pageSize");

  if (!pageStr.empty()) page = std::stoi(pageStr);
  if (!pageSizeStr.empty()) pageSize = std::stoi(pageSizeStr);
  if (page < 1) page = 1;
  if (pageSize > 20) pageSize = 20;

  const auto &placeOfStudyFilter = req->getParameter("place_of_study");
  const auto &facultyOfEducFilter = req->getParameter("faculty_of_educ");
  const auto &graduationYearFilter = req->getParameter("graduation_year");
  const auto &placeFilter = req->getParameter("place");
  const auto &searchStatusIdFilter = req->getParameter("search_status_id");
  const auto &experienceStatusIdFilter = req->getParameter("experience_status_id");
  const auto &workScheduleIdFilter = req->getParameter("work_schedule_status_id");

  const int offset = (page - 1) * pageSize;
  const auto &dbClient = app().getDbClient();

  std::string sql = "SELECT user_id, place_of_study, faculty_of_educ, experience_status_id FROM candidates WHERE 1=1";
  std::vector<std::string> paramStrings;

  if (!placeOfStudyFilter.empty()) {
    sql += " AND place_of_study = ?";
    paramStrings.push_back(placeOfStudyFilter);
  }
  if (!facultyOfEducFilter.empty()) {
    sql += " AND faculty_of_educ = ?";
    paramStrings.push_back(facultyOfEducFilter);
  }
  if (!graduationYearFilter.empty()) {
    try {
      std::stoi(graduationYearFilter);
      sql += " AND graduation_year = ?";
      paramStrings.push_back(graduationYearFilter);
    } catch (...) {
      Json::Value err;
      err["error"] = "Invalid graduation_year format";
      auto resp = HttpResponse::newHttpJsonResponse(err);
      resp->setStatusCode(k400BadRequest);
      callback(resp);
      return;
    }
  }
  if (!placeFilter.empty()) {
    sql += " AND place = ?";
    paramStrings.push_back(placeFilter);
  }
  if (!searchStatusIdFilter.empty()) {
    try {
      std::stoi(searchStatusIdFilter);
      sql += " AND search_status_id = ?";
      paramStrings.push_back(searchStatusIdFilter);
    } catch (...) {
      Json::Value err;
      err["error"] = "Invalid search_status_id format";
      auto resp = HttpResponse::newHttpJsonResponse(err);
      resp->setStatusCode(k400BadRequest);
      callback(resp);
      return;
    }
  }
  if (!experienceStatusIdFilter.empty()) {
    try {
      std::stoi(experienceStatusIdFilter);
      sql += " AND experience_status_id = ?";
      paramStrings.push_back(experienceStatusIdFilter);
    } catch (...) {
      Json::Value err;
      err["error"] = "Invalid experience_status_id format";
      auto resp = HttpResponse::newHttpJsonResponse(err);
      resp->setStatusCode(k400BadRequest);
      callback(resp);
      return;
    }
  }
  if (!workScheduleIdFilter.empty()) {
    try {
      std::stoi(workScheduleIdFilter);
      sql += " AND work_schedule_status_id = ?";
      paramStrings.push_back(workScheduleIdFilter);
    } catch (...) {
      Json::Value err;
      err["error"] = "Invalid work_schedule_status_id format";
      auto resp = HttpResponse::newHttpJsonResponse(err);
      resp->setStatusCode(k400BadRequest);
      callback(resp);
      return;
    }
  }

  sql += " LIMIT " + std::to_string(pageSize) + " OFFSET " + std::to_string(offset);

  auto binder = *dbClient << sql;
  for (const auto &param : paramStrings) {
    binder << param;
  }

  binder >> [callback, dbClient, page, pageSize](const drogon::orm::Result &candidatesResult) mutable {
    std::vector<int> candidateIds;
    for (const auto &row : candidatesResult) {
      candidateIds.push_back(row["user_id"].as<int>());
    }

    if (candidateIds.empty()) {
      Json::Value resp;
      resp["candidates"] = Json::arrayValue;
      resp["page"] = page;
      resp["pageSize"] = pageSize;
      resp["count"] = 0;
      callback(HttpResponse::newHttpJsonResponse(resp));
      return;
    }

    std::string placeholders;
    for (size_t i = 0; i < candidateIds.size(); ++i) {
      if (i > 0) placeholders += ",";
      placeholders += std::to_string(candidateIds[i]);
    }

    const std::string expSql =
        "SELECT cs.user_id, exp.name FROM candidates cs "
        "JOIN experience exp ON cs.experience_status_id = exp.id "
        "WHERE cs.user_id IN (" + placeholders + ")";

    dbClient->execSqlAsync(
        expSql,
        [callback, candidatesResult, candidateIds, page, pageSize](const drogon::orm::Result &expResult) {
          std::unordered_map<int, std::string> candidateExp;
          for (const auto &row : expResult) {
            candidateExp[row["user_id"].as<int>()] = row["name"].as<std::string>();
          }

          Json::Value resp;
          Json::Value candidatesJson(Json::arrayValue);
          for (const auto &row : candidatesResult) {
            const int cid = row["user_id"].as<int>();
            Json::Value item;
            item["id"] = cid;
            item["place"] = row["place_of_study"].as<std::string>();
            item["faculty"] = row["faculty_of_educ"].as<std::string>();
            item["experience"] = candidateExp[cid];
            item["user_id"] = cid;
            candidatesJson.append(item);
          }

          resp["candidates"] = candidatesJson;
          resp["page"] = page;
          resp["pageSize"] = pageSize;
          resp["count"] = static_cast<int>(candidateIds.size());
          callback(HttpResponse::newHttpJsonResponse(resp));
        },
        [callback](const drogon::orm::DrogonDbException &e) {
          Json::Value err;
          err["error"] = "Database error on experience query";
          auto resp = HttpResponse::newHttpJsonResponse(err);
          resp->setStatusCode(k500InternalServerError);
          callback(resp);
        });
  } >> [callback](const drogon::orm::DrogonDbException &e) {
    Json::Value err;
    err["error"] = "Database error on query";
    auto resp = HttpResponse::newHttpJsonResponse(err);
    resp->setStatusCode(k500InternalServerError);
    callback(resp);
  };
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
    // TODO: make vacId string by def
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

    // works SYNC
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

void resources::updateVacancy(
        const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const int vacId
    ){
        const auto& jsonReq = req->getJsonObject();
        Json::Value jsonResp;
        const auto& session  = req->session();

        if (!jsonReq){
            jsonResp["error"] = "No data to patch!";
            const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
            return;
        }

        //findOne is not noexcept!
        // it should throw if there are != 1 vacancy w this ID
        try{
        const auto& dbClient = app().getDbClient();
        const auto criteria = orm::Criteria(drogon_model::default_db::Vacancies::Cols::_id, orm::CompareOperator::EQ, std::to_string(vacId));
        orm::Mapper<drogon_model::default_db::Vacancies> mapper(dbClient);

        auto vac = mapper.findOne(criteria);

        //does not cahnge ID! But may change empl id... (fixed)
        vac.updateByJson(*jsonReq);
        const int emplId = std::stoi(session->get<std::string>("user_id"));
        vac.setEmployerId(emplId);
        mapper.update(vac);
        jsonResp["message"] = "Vacancy has been updated";
        const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
        resp->setStatusCode(drogon::k200OK);
        callback(resp);

        }catch(const orm::DrogonDbException& err){
            jsonResp["error"] = err.base().what();
            LOG_ERROR << err.base().what();
            const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(HttpStatusCode::k500InternalServerError);
            callback(resp);
        }
    }

void resources::getVacanciesCards(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

    int page = 1;
    int pageSize = 20;

    const auto &pageStr = req->getParameter("page");
    const auto &pageSizeStr = req->getParameter("pageSize");

    if (!pageStr.empty()) page = std::stoi(pageStr);
    if (!pageSizeStr.empty()) pageSize = std::stoi(pageSizeStr);
    if (page < 1) page = 1;
    if (pageSize > 20) pageSize = 20;

    const auto &salaryFilter = req->getParameter("salary");
    const auto &placeFilter = req->getParameter("place");
    const auto &educPlaceFilter = req->getParameter("educ_place");
    const auto &experienceStatusIdFilter = req->getParameter("experience_status_id");
    const auto &workScheduleStatusIdFilter = req->getParameter("work_schedule_status_id");
    const auto &educStatusIdFilter = req->getParameter("educ_status_id");
    const auto &remotenessStatusIdFilter = req->getParameter("remoteness_status_id");

    const int offset = (page - 1) * pageSize;
    const auto &dbClient = app().getDbClient();

    std::string sql = "SELECT id, name, salary, place, work_schedule_status_id, employer_id, remoteness_status_id, status FROM vacancies WHERE 1=1";
    std::vector<std::variant<std::string, int>> params;

    try {
        if (!salaryFilter.empty()) {
            int salary = std::stoi(salaryFilter);
            sql += " AND salary >= ?";
            params.emplace_back(salary);
        }

        if (!placeFilter.empty()) {
            sql += " AND place = ?";
            params.emplace_back(placeFilter);
        }

        if (!educPlaceFilter.empty()) {
            sql += " AND educ_place = ?";
            params.emplace_back(educPlaceFilter);
        }

        if (!experienceStatusIdFilter.empty()) {
            int val = std::stoi(experienceStatusIdFilter);
            sql += " AND experience_status_id = ?";
            params.emplace_back(val);
        }

        if (!workScheduleStatusIdFilter.empty()) {
            int val = std::stoi(workScheduleStatusIdFilter);
            sql += " AND work_schedule_status_id = ?";
            params.emplace_back(val);
        }

        if (!educStatusIdFilter.empty()) {
            int val = std::stoi(educStatusIdFilter);
            sql += " AND educ_status_id = ?";
            params.emplace_back(val);
        }

        if (!remotenessStatusIdFilter.empty()) {
            int val = std::stoi(remotenessStatusIdFilter);
            sql += " AND remoteness_status_id = ?";
            params.emplace_back(val);
        }
    } catch (...) {
        Json::Value err;
        err["error"] = "Invalid filter parameter format";
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    sql += " LIMIT " + std::to_string(pageSize) + " OFFSET " + std::to_string(offset);

    auto binder = *dbClient << sql;
    for (const auto &param : params) {
        if (std::holds_alternative<int>(param))
            binder << std::get<int>(param);
        else
            binder << std::get<std::string>(param);
    }

    binder >> [callback, dbClient, page, pageSize](const orm::Result &vacanciesResult) mutable {
        std::set<int> emplIds;
        for (const auto &row : vacanciesResult)
            emplIds.insert(row["employer_id"].as<int>());

        if (emplIds.empty()) {
            Json::Value resp;
            resp["vacancies"] = Json::arrayValue;
            resp["page"] = page;
            resp["pageSize"] = pageSize;
            resp["count"] = 0;
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        std::string placeholders;
        for (auto it = emplIds.begin(); it != emplIds.end(); ++it) {
            if (it != emplIds.begin()) placeholders += ",";
            placeholders += std::to_string(*it);
        }

        const std::string companySql =
            "SELECT user_id, company_name FROM employers WHERE user_id IN (" + placeholders + ")";

        dbClient->execSqlAsync(
            companySql,
            [callback, vacanciesResult, page, pageSize](const orm::Result &companiesResult) mutable {
                std::unordered_map<int, std::string> companies;
                for (const auto &row : companiesResult)
                    companies[row["user_id"].as<int>()] = row["company_name"].as<std::string>();

                Json::Value resp;
                Json::Value vacsJson(Json::arrayValue);
                for (const auto &row : vacanciesResult) {
                    const int emplId = row["employer_id"].as<int>();
                    Json::Value vacJson;
                    vacJson["id"] = row["id"].as<int>();
                    vacJson["company_name"] = companies[emplId];
                    vacJson["salary"] = row["salary"].as<std::string>();
                    vacJson["name"] = row["name"].as<std::string>();
                    vacJson["place"] = row["place"].as<std::string>();
                    vacJson["status"] = row["status"].as<std::string>();
                    vacJson["work_schedule_status_id"] = row["work_schedule_status_id"].as<std::string>();
                    vacJson["remoteness_status_id"] = row["remoteness_status_id"].as<std::string>();
                    vacsJson.append(vacJson);
                }

                resp["vacancies"] = vacsJson;
                resp["page"] = page;
                resp["pageSize"] = pageSize;
                resp["count"] = (int)vacanciesResult.size();
                callback(HttpResponse::newHttpJsonResponse(resp));
            },
            [callback](const orm::DrogonDbException &e) {
                Json::Value err;
                err["error"] = "Database error on companies query";
                auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k500InternalServerError);
                callback(resp);
            }
        );
    } >> [callback](const orm::DrogonDbException &e) {
        Json::Value err;
        err["error"] = "Database error on vacancies query";
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    };
}



void resources::getCandidateInfo(
    const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string userId){

    const auto& dbClient = app().getDbClient();
    const std::string sql = "SELECT name, surname, email, phone_num, place, place_of_study, faculty_of_educ, experience_status_id FROM candidates WHERE user_id = ?";
    dbClient->execSqlAsync(sql,
        [callback, &userId](const orm::Result& candidate){
            Json::Value jsonResp;

            if (candidate.empty()){
                jsonResp["error"] = "User not found";\
                LOG_ERROR << "no user in db with id: " + userId;
                const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
                resp->setStatusCode(drogon::k404NotFound);
                callback(resp);
                return;
            }

            if (candidate.size() > 1){
                jsonResp["error"] = "User id is ambigious";
                LOG_ERROR << "more than 1 user w id: " + userId;
                const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
                resp->setStatusCode(drogon::k500InternalServerError);
                callback(resp);
                return;
            }

            const auto& row = candidate[0];

            jsonResp["name"] = row["name"].as<std::string>();
            jsonResp["surname"] = row["surname"].as<std::string>();
            jsonResp["email"] = row["email"].as<std::string>();
            jsonResp["phone_num"] = row["phone_num"].as<std::string>();
            jsonResp["place"] = row["place"].as<std::string>();
            jsonResp["study_info"] = row["place_of_study"].as<std::string>() + " " + row["faculty_of_educ"].as<std::string>();
            jsonResp["experience_status_id"] = row["experience_status_id"].as<int>();

            const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp -> setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const orm::DrogonDbException& err){
            Json::Value jsonResp;
            LOG_ERROR << err.base().what();
            jsonResp["error"] = "Database error on candidate query";

            const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);},
        userId);
    }
