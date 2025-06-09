#include "api_v1_Auth.h"
#include "Candidates.h"
#include "Employers.h"
#include "Users.h"
#include "drogon/orm/Criteria.h"
#include "drogon/orm/Mapper.h"
#include "trantor/utils/Logger.h"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/Exception.h>

using namespace api::v1;

// Add definition of your processing function here
void Auth::login(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback) {

  const auto &json = req->getJsonObject();

  if (!json || !json->isMember("email") || !json->isMember("password") ||
      (*json)["email"].asString().empty() ||
      (*json)["password"].asString().empty()) {
    const auto resp = HttpResponse::newHttpJsonResponse(
        Json::Value{{"error", "Invalid request format"}});
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  const std::string &email = (*json)["email"].asString();
  const std::string &password = (*json)["password"].asString();

  const std::string sqlString =
      "SELECT id, password, status FROM " + USERS_TABLE + " WHERE email = ?";
  LOG_DEBUG << "Executing SQL query: " << sqlString << " with email: " << email
            << "\n";
  const auto &dbClient = app().getDbClient();
  auto callbackPtr =
      std::make_shared<std::function<void(const HttpResponsePtr &)>>(
          std::move(callback));
  dbClient->execSqlAsync(
      sqlString,
      [=](const orm::Result &result) {
        if (result.empty()) {
          Json::Value json;
          json["error"] = "User not found";
          const auto resp = HttpResponse::newHttpJsonResponse(json);
          resp->setStatusCode(k401Unauthorized);
          (*callbackPtr)(resp);
          return;
        }
        const auto &row = result[0];
        if (const auto dbPassword = row["password"].as<std::string>();
            password != dbPassword) {
          Json::Value json;
          json["error"] = "Wrong password";
          const auto resp = HttpResponse::newHttpJsonResponse(json);
          resp->setStatusCode(k401Unauthorized);
          (*callbackPtr)(resp);
          return;
        }

        const auto status = row["status"].as<std::string>();
        LOG_DEBUG << "User status: " << status << "\n";

        if (status != CAND_STATUS && status != EMPL_STATUS) {
          Json::Value json;
          json["error"] = "unexpected user status in DB";
          const auto resp = HttpResponse::newHttpJsonResponse(json);
          LOG_DEBUG << "unexpected user status in DB\n";
          resp->setStatusCode(k500InternalServerError);
          (*callbackPtr)(resp);
          return;
        }

        try {
          const auto session = req->session();
          LOG_DEBUG << "Got a session: User id: " << row["id"].as<std::string>()
                    << " Status: " << status << "\n";

          session->insert("authenticated", true);
          session->insert("user_id", row["id"].as<std::string>());
          session->insert("user_status", status);

          Json::Value jsonResp;
          jsonResp["status"] = "success";
          jsonResp["session_id"] = session->sessionId();
          jsonResp["status_user"] = status;

          const auto resp = HttpResponse::newHttpJsonResponse(jsonResp);

          (*callbackPtr)(resp);
        } catch (...) {
          Json::Value json;
          json["error"] = "Session creation failed";
          const auto resp = HttpResponse::newHttpJsonResponse(json);
          LOG_DEBUG << "Session creation failed\n";
          resp->setStatusCode(drogon::k500InternalServerError);
          (*callbackPtr)(resp);
        }
      },
      [=](const orm::DrogonDbException &e) {
        Json::Value json;
        json["error"] = "Database error";
        const auto resp = HttpResponse::newHttpJsonResponse(json);
        LOG_DEBUG << "Database error\n";
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
      },
      email);
}

void Auth::registerUser(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  const auto &jsonReq = req->getJsonObject();
  if (!jsonReq || !jsonReq->isMember("status") || (*jsonReq)["status"].asString().empty()) {
    Json::Value json;
    json["error"] = "Missing 'status' parameter";
    const auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  if (!jsonReq) {
    Json::Value json;
    json["error"] = "Missing JSON body";
    const auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  if (!jsonReq->isMember("email") || !jsonReq->isMember("password")
      || !(*jsonReq)["email"].isString() || !(*jsonReq)["password"].isString()) {
    Json::Value json;
    json["error"] = "Invalid JSON body";
    const auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }
  Json::Value jsonResp;
  const auto &email = (*jsonReq)["email"].asString();
  const auto &password = (*jsonReq)["password"].asString();

  const auto &dbClient = app().getDbClient();

  auto usersMapper = orm::Mapper<drogon_model::default_db::Users>(dbClient);
  orm::Criteria findUserCriteria{drogon_model::default_db::Users::Cols::_email,
                                 orm::CompareOperator::EQ, email};

  if (auto users = usersMapper.findBy(findUserCriteria); !users.empty()) {
    jsonResp["error"] = "Email already exists!";
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(k409Conflict);
    callback(resp);
    return;
  }

  drogon_model::default_db::Users user;
  user.setEmail(email);
  user.setPassword(password);
  user.setStatus((*jsonReq)["status"].asString());
  usersMapper.insert({user});

  if ((*jsonReq)["status"].asString() == CAND_STATUS) {
    auto candMapper =
        orm::Mapper<drogon_model::default_db::Candidates>(dbClient);
    orm::Criteria findCandidateCriteria{
        drogon_model::default_db::Candidates::Cols::_email,
        orm::CompareOperator::EQ, email};

    if (const auto candidates = candMapper.findBy(findCandidateCriteria);
        (!candidates.empty())) {
      jsonResp["error"] = "Email already exists in candidates dataset";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(k409Conflict);
      callback(resp);
      return;
    }

    drogon_model::default_db::Candidates candidate;
    const std::string &name = (*jsonReq)["name"].asString();
    const std::string &surname = (*jsonReq)["surname"].asString();

    candidate.setName(name);
    candidate.setSurname(surname);
    candidate.setEmail(email);
    candidate.setUserId(*user.getId());

    candMapper.insert({candidate});
  }

  if ((*jsonReq)["status"].asString() == EMPL_STATUS) {
    auto emplMapper =
        orm::Mapper<drogon_model::default_db::Employers>(dbClient);
    orm::Criteria findEmployerCriteria{
        drogon_model::default_db::Employers::Cols::_email,
        orm::CompareOperator::EQ, email};

    if (const auto employers = emplMapper.findBy(findEmployerCriteria);
        !employers.empty()) {
      jsonResp["error"] = "Email already exists in employers dataset";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(k409Conflict);
      callback(resp);
      return;
    }

    drogon_model::default_db::Employers employer;

    const std::string &companyName = (*jsonReq)["company_name"].asString();

    employer.setEmail(email);
    employer.setCompanyName(companyName);
    employer.setUserId(*user.getId());

    emplMapper.insert({employer});
  }

  LOG_DEBUG << " Registered user. Id: " << (*user.getId()) << "\n";
  jsonResp["message"] = "User registered successfully";
  auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
  resp->setStatusCode(k201Created);
  callback(resp);
}
