#include "api_v1_profile.h"
#include "Candidates.h"
#include "Employers.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"
#include "drogon/orm/Criteria.h"
#include "drogon/orm/Mapper.h"
#include "api_v1_Auth.h"
using namespace api::v1;

// Add definition of your processing function here
void profile::getProfile(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    auto session = req->session();

    auto userStatus = req->session()->get<std::string>("user_status");
    auto userId = req->session()->get<std::string>("user_id");
    auto dbClient = app().getDbClient();

    if (userStatus == CAND_STATUS){
        auto mapper = drogon::orm::Mapper<drogon_model::default_db::Candidates>(dbClient);
        orm::Criteria findCriteria{drogon_model::default_db::Candidates::Cols::_id,orm::CompareOperator::EQ,userId };

        auto candidates = mapper.findBy(findCriteria);

        if (candidates.empty()){
            Json::Value json;
            json["error"] = "Candidate not found";
            auto resp = HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::k404NotFound);

            callback(resp);
            return;
        }

        Json::Value json;

        auto candidate = candidates.front();
        json = candidate.toJson();
        auto resp = HttpResponse::newHttpJsonResponse(json);

        callback(resp);
    }
    else if (userStatus == EMPL_STATUS){
        auto mapper = drogon::orm::Mapper<drogon_model::default_db::Employers>(dbClient);
        orm::Criteria findCriteria{drogon_model::default_db::Employers::Cols::_id, orm::CompareOperator::EQ, userId};

        auto empls = mapper.findBy(findCriteria);

        if (empls.empty()){
            Json::Value json;
            json["error"] = "Employer not found";

            auto resp = HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(k404NotFound);

            callback(resp);
            return;
        }

        Json::Value json;

        auto empl = empls.front();
        json = empl.toJson();
        auto resp = HttpResponse::newHttpJsonResponse(json);

        callback(resp);
    }else{
        Json::Value json;
        json["error"] = "Cannot get user status from session";

        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(drogon::k400BadRequest);

        callback(resp);
    }
}
