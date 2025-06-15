#pragma once

#include "drogon/HttpTypes.h"
#include <drogon/HttpController.h>

using namespace drogon;

namespace api::v1 {

class Notifications : public drogon::HttpController<Notifications> {
    static void
    candRespond(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    static void
    emplRespond(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    static void getResponsesForEmpl(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);

    static void getInvitationsForCand(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);

    static void
    deleteResponse(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback);

    static void sendAcceptanceEmail(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback);

  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Notifications::candRespond, "/candRespond",
               drogon::Post); // path is /api/v1/Notifications/candRespond
    METHOD_ADD(Notifications::emplRespond, "/emplRespond",
               drogon::Post); // path is /api/v1/Notifications/emplRespond
    METHOD_ADD(
        Notifications::getResponsesForEmpl, "/getResponsesForEmpl",
        drogon::Get); // path is /api/v1/Notifications/getResponsesForEmpl
    METHOD_ADD(
        Notifications::getInvitationsForCand, "/getInvitationsForCand",
        drogon::Post); // path is /api/v1/Notifications/getInvitationsForCand
    METHOD_ADD(Notifications::deleteResponse, "/deleteResponse",
               drogon::Delete); // path is /api/v1/Notifications/deleteResponse
    METHOD_ADD(
        Notifications::sendAcceptanceEmail, "/sendAcceptanceEmail",
        drogon::Post); // path is /api/v1/Notifications/sendAcceptanceEmail

    METHOD_LIST_END
};

} // namespace api::v1
