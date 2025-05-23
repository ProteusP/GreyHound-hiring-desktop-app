#include "api_v1_profile.h"
#include "Candidates.h"
#include "Employers.h"
#include "api_v1_Auth.h"
#include "drogon/HttpAppFramework.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/orm/Criteria.h"
#include "drogon/orm/Mapper.h"
using namespace api::v1;

// Add definition of your processing function here
void profile::getProfile(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  const auto &session = req->session();
  Json::Value jsonResp;

  const auto &userStatus = session->get<std::string>("user_status");
  const auto &userId = session->get<std::string>("user_id");
  const auto &dbClient = app().getDbClient();

  if (userStatus == CAND_STATUS) {
    auto mapper =
        drogon::orm::Mapper<drogon_model::default_db::Candidates>(dbClient);
    orm::Criteria findCriteria{drogon_model::default_db::Candidates::Cols::_user_id,
                               orm::CompareOperator::EQ, userId};

    const auto &candidates = mapper.findBy(findCriteria);

    if (candidates.empty()) {
      jsonResp["error"] = "Candidate not found";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(drogon::k404NotFound);

      callback(resp);
      return;
    }

    const auto &candidate = candidates.front();
    jsonResp = candidate.toJson();
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
  } else if (userStatus == EMPL_STATUS) {
    auto mapper =
        drogon::orm::Mapper<drogon_model::default_db::Employers>(dbClient);
    orm::Criteria findCriteria{drogon_model::default_db::Employers::Cols::_user_id,
                               orm::CompareOperator::EQ, userId};

    const auto &employers = mapper.findBy(findCriteria);

    if (employers.empty()) {
      jsonResp["error"] = "Employer not found";

      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(k404NotFound);

      callback(resp);
      return;
    }

    const auto &empl = employers.front();
    jsonResp = empl.toJson();
    //DEBUG
    try {
      LOG_DEBUG<<"empl json string: "<< jsonResp.toStyledString();
    }catch (...) {
      LOG_DEBUG<<"cant parse json";
    }
    //DEBUG
    saveUserProfile(userId,jsonResp);
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(drogon::k200OK);

    callback(resp);
  } else {
    jsonResp["error"] = "Cannot get user status from session";

    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(drogon::k400BadRequest);

    callback(resp);
  }
}

void profile::patchProfile(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  const auto &session = req->session();
  Json::Value jsonResp;

  const auto &userStatus = session->get<std::string>("user_status");
  const auto &userId = session->get<std::string>("user_id");
  const auto &dbClient = app().getDbClient();
  auto jsonReq = req->getJsonObject();

  if (!jsonReq) {
    jsonResp["error"] = "Invalid JSON";
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(drogon::k400BadRequest);
    callback(resp);
    return;
  }
  // findOne is not noexcept...
  try {
    if (userStatus == CAND_STATUS) {
      drogon::orm::Mapper<drogon_model::default_db::Candidates> mapper(
          dbClient);
      auto candidate = mapper.findOne(
          orm::Criteria{drogon_model::default_db::Candidates::Cols::_user_id,
                        orm::CompareOperator::EQ, userId});
      // Does not change ID
      candidate.updateByJson(*jsonReq);

      mapper.update(candidate);

      jsonResp["message"] = "Candidate updated";
      jsonResp["data"] = candidate.toJson();
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(drogon::k200OK);
      callback(resp);
    } else if (userStatus == EMPL_STATUS) {
      drogon::orm::Mapper<drogon_model::default_db::Employers> mapper(dbClient);
      auto employer = mapper.findOne(
          orm::Criteria{drogon_model::default_db::Employers::Cols::_user_id,
                        orm::CompareOperator::EQ, userId});

      // Does not change ID
      employer.updateByJson(*jsonReq);

      mapper.update(employer);

      jsonResp["message"] = "Employer updated";
      jsonResp["data"] = employer.toJson();
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(drogon::k200OK);
      callback(resp);
    } else {
      jsonResp["error"] = "Unknown user status";
      auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
      resp->setStatusCode(drogon::k400BadRequest);
      callback(resp);
    }
  } catch (const drogon::orm::DrogonDbException &e) {
    jsonResp["error"] = e.base().what();
    auto resp = HttpResponse::newHttpJsonResponse(jsonResp);
    resp->setStatusCode(drogon::k500InternalServerError);
    callback(resp);
  }
}

void saveUserProfile(const std::string &id, const Json::Value& data) {
  const auto redisClientPtr = app().getRedisClient();

  const std::string key = "profile:" + id;
  const std::string strData = data.asString();

  try
  {
    auto res = redisClientPtr->execCommandSync<std::string>(
        [](const nosql::RedisResult &r){
            return r.asString();
        },
        "set %s %s",
        key.data(), strData.data()
    );
  }
  catch (const nosql::RedisException & err)
  {
  }
  catch (const std::exception & err)
  {
  }

  LOG_DEBUG << "Saving user profile: " << key;
}
