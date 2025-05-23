/**
 *
 *  api_v1_checkAuthFilter.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;
namespace api
{
namespace v1
{

class checkAuthFilter : public HttpFilter<checkAuthFilter>
{
  public:
    checkAuthFilter() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

}
}
