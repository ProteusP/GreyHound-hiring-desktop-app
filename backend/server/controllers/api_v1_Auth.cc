#include "api_v1_Auth.h"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/Exception.h>

using namespace api::v1;

// Add definition of your processing function here
void Auth::loginAsCandidate(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto json = req->getJsonObject();

    if (!json || !json->isMember("email") || !json->isMember("password")){

        auto resp = HttpResponse::newHttpJsonResponse(
            Json::Value{{"error", "Invalid req format"}}
        );
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string email = (*json)["email"].asString();
    std::string password = (*json)["password"].asString();

    auto client = app().getDbClient();

    client->execSqlAsync(
        "SELECT id, password FROM candidates WHERE email = ?",
        [=,callback = std::move(callback)](const drogon::orm::Result& result) {
            if (result.empty()){
                auto resp = HttpResponse::newHttpJsonResponse(
                    Json::Value{{"error", "User not found"}}
                );
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
                return;
            }

            auto row = result[0];

            std::string id = row["id"].as<std::string>();
            std::string pwdFromDB = row["password"].as<std::string>();

            if (password == pwdFromDB){
                req->session()->insert("authenticated", true);
                req->session()->insert("user_id", id);

                Json::Value resp;
                resp["status"] = "succes";
                resp["user_id"] = id;
                callback(HttpResponse::newHttpJsonResponse(resp));
            }
            else{
                auto resp = HttpResponse::newHttpJsonResponse(
                    Json::Value{{"error", "Wrong password"}}
                );
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
            }
        },
        [](const orm::DrogonDbException &e){
            std::cerr << "error: "<<e.base().what() << std::endl;
        },
        email);
}


void Auth::loginAsEmpl(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto json = req->getJsonObject();

    if (!json || !json->isMember("email") || !json->isMember("password")){

        auto resp = HttpResponse::newHttpJsonResponse(
            Json::Value{{"error", "Invalid req format"}}
        );
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string email = (*json)["email"].asString();
    std::string password = (*json)["password"].asString();

    auto client = app().getDbClient();

    client->execSqlAsync(
        "SELECT id, password FROM employers WHERE email = ?",
        [=,callback = std::move(callback)](const drogon::orm::Result& result) {
            if (result.empty()){
                auto resp = HttpResponse::newHttpJsonResponse(
                    Json::Value{{"error", "User not found"}}
                );
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
                return;
            }

            auto row = result[0];

            std::string id = row["id"].as<std::string>();
            std::string pwdFromDB = row["password"].as<std::string>();

            if (password == pwdFromDB){
                req->session()->insert("authenticated", true);
                req->session()->insert("user_id", id);

                Json::Value resp;
                resp["status"] = "succes";
                resp["user_id"] = id;
                callback(HttpResponse::newHttpJsonResponse(resp));
            }
            else{
                auto resp = HttpResponse::newHttpJsonResponse(
                    Json::Value{{"error", "Wrong password"}}
                );
                resp->setStatusCode(drogon::k401Unauthorized);
                callback(resp);
            }
        },
        [](const orm::DrogonDbException &e){
            std::cerr << "error: "<<e.base().what() << std::endl;
        },
        email);
}
