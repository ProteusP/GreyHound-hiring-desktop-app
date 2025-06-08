/**
 *
 *  api_v1_checkAuthFilter.cc
 *
 */

#include "api_v1_checkAuthFilter.h"
#include "drogon/HttpRequest.h"
#include "drogon/HttpResponse.h"
#include "drogon/HttpTypes.h"

using namespace drogon;
using namespace api::v1;

void checkAuthFilter::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    auto session = req->session();
    /*We would need to add a constant for the field in the session */
    if (!session || !session->find("authenticated") || !session->get<bool>("authenticated")){
        Json::Value json;
        json["error"] = "Unauthorized";

        auto resp = HttpResponse::newHttpJsonResponse(json);
        resp->setStatusCode(drogon::k401Unauthorized);
        fcb(resp);
        return;
    }
    //passed
    fccb();
}
