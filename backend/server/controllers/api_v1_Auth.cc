#include "api_v1_Auth.h"
#include "Candidates.h"
#include "Employers.h"
#include "drogon/orm/Criteria.h"
#include "drogon/orm/Mapper.h"
#include "trantor/utils/Logger.h"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/Exception.h>
#include <exception>

using namespace api::v1;

// Add definition of your processing function here
void Auth::login(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback) {

  auto status = req->getParameter("status");
  if (status.empty()) {
    Json::Value json;
    json["error"] = "Missing 'status' parameter";
    auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  const auto &json = req->getJsonObject();

  if (!json || !json->isMember("email") || !json->isMember("password")) {
    auto resp = HttpResponse::newHttpJsonResponse(
        Json::Value{{"error", "Invalid request format"}});
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  std::string email = (*json)["email"].asString();
  std::string password = (*json)["password"].asString();

  std::string table;
  if (status == CAND_STATUS) {
    table = "candidates";
  } else if (status == EMPL_STATUS) {
    table = "employers";
  } else {
    auto resp = HttpResponse::newHttpJsonResponse(
        Json::Value{{"error", "Invalid status parameter"}});
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  std::string sqlString =
      "SELECT id, password FROM " + table + " WHERE email = ?";
  LOG_DEBUG << "Executing SQL query: " << sqlString << "with email: " << email
            << "\n";
  auto client = app().getDbClient();
  client->execSqlAsync(
      sqlString,
      [=, callback = std::move(callback)](const orm::Result &result) {
        if (result.empty()) {
          auto resp = HttpResponse::newHttpJsonResponse(
              Json::Value{{"error", "User not found"}});
          resp->setStatusCode(k401Unauthorized);
          callback(resp);
          return;
        }

        auto row = result[0];
        std::string dbPassword = row["password"].as<std::string>();

        if (password != dbPassword) {
          auto resp = HttpResponse::newHttpJsonResponse(
              Json::Value{{"error", "Wrong password"}});
          resp->setStatusCode(k401Unauthorized);
          callback(resp);
          return;
        }

        try {
          auto session = req->session();

          session->insert("authenticated", true);
          session->insert("user_id", row["id"].as<std::string>());
          session->insert("user_status", status);

          Json::Value json;
          json["status"] = "success";
          json["session_id"] = session->sessionId();

          auto resp = HttpResponse::newHttpJsonResponse(json);

          callback(resp);
        } catch (const std::exception &e) {
          Json::Value json{{"error", "Session creation failed"}};
          auto resp = HttpResponse::newHttpJsonResponse(json);
          resp->setStatusCode(drogon::k500InternalServerError);

          callback(resp);
        }
      },
      [=, callback = std::move(callback)](const orm::DrogonDbException &e) {
        auto resp = HttpResponse::newHttpJsonResponse(
            Json::Value{{"error", "Database error"}});
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
      },
      email);
}

void Auth::registerUser(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {

  auto status = req->getParameter("status");

  if (status.empty()) {
    Json::Value json;
    json["error"] = "Missing 'status' parameter";
    auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  const auto &jsonReq = req->getJsonObject();

  if (!jsonReq) {
    Json::Value json{{"error", "Missing JSON body"}};
    auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  if (!(*jsonReq)["email"].isString() || !(*jsonReq)["password"].isString()) {
    Json::Value json{{"error", "Invalid JSON body"}};
    auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }
  Json::Value jsonResp;
  auto email = (*jsonReq)["email"].asString();
  auto password = (*jsonReq)["password"].asString();

  const auto &client = app().getDbClient();

  if (status == CAND_STATUS) {
    auto mapper = orm::Mapper<drogon_model::default_db::Candidates>(client);
    orm::Criteria findCriteria{
        drogon_model::default_db::Candidates::Cols::_email,
        orm::CompareOperator::EQ, email};
    auto candidates = mapper.findBy(findCriteria);

    if (!candidates.empty()) {
      jsonResp["error"] = "Email already exists";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(k409Conflict);
      callback(resp);
      return;
    }

    drogon_model::default_db::Candidates candidate;
    candidate.setEmail(email);
    candidate.setPassword(password);
    mapper.insert(candidate);
    jsonResp["message"] = "User registered successfully";
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(k201Created);
    callback(resp);
  } else if (status == EMPL_STATUS) {
    auto mapper = orm::Mapper<drogon_model::default_db::Employers>(client);
    orm::Criteria findCriteria{
        drogon_model::default_db::Employers::Cols::_email,
        orm::CompareOperator::EQ, email};
    auto candidates = mapper.findBy(findCriteria);

    if (!candidates.empty()) {
      jsonResp["error"] = "Email already exists";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(k409Conflict);
      callback(resp);
      return;
    }

    drogon_model::default_db::Employers empl;
    empl.setEmail(email);
    empl.setPassword(password);
    mapper.insert(empl);
    jsonResp["message"] = "User registered successfully";
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(k201Created);
    callback(resp);
  }
}
