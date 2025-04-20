#include "api_v1_Auth.h"
#include "trantor/utils/Logger.h"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/Exception.h>

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

    auto json = req->getJsonObject();

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
    if (status == "candidate") {
        table = "candidates";
    } else if (status == "empl") {
        table = "employers";
    } else {
        auto resp = HttpResponse::newHttpJsonResponse(
            Json::Value{{"error", "Invalid status parameter"}});
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::string sqlString = "SELECT id, password FROM " + table + " WHERE email = ?";
    LOG_DEBUG << "Executing SQL query: " << sqlString << "with email: " << email;
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

            // Успешная аутентификация
            req->session()->insert("authenticated", true);
            req->session()->insert("user_id", row["id"].as<std::string>());

            Json::Value json;
            json["status"] = "success";
            json["user_id"] = row["id"].as<std::string>();

            auto resp = HttpResponse::newHttpJsonResponse(json);

            callback(resp);
        },
        [callback](const orm::DrogonDbException &e) {
            auto resp = HttpResponse::newHttpJsonResponse(
                Json::Value{{"error", "Database error"}});
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
        },
        email);
}
