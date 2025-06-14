#include "api_v1_Notifications.h"
#include "Candidates.h"
#include "Employers.h"
#include "Invitations.h"
#include "Responces.h"
#include "Vacancies.h"
#include "drogon/orm/Criteria.h"
#include "drogon/orm/Mapper.h"
#include "trantor/utils/Logger.h"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/Exception.h>

using namespace api::v1;

// Add definition of your processing function here
void Notifications::candRespond(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto &json = req->getJsonObject();

    if (!json || !json->isMember("vacancy_id") ||
    !json->isMember("candidate_id") ||
    !json->isMember("employer_id")) {
        Json::Value err;
        err["error"] =
            "Invalid request format: missing vacancy_id or candidate_id";
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    const int vacancy_id = (*json)["vacancy_id"].asInt();
    const int employer_id = (*json)["employer_id"].asInt();
    const int candidate_id = (*json)["candidate_id"].asInt();

    const auto &dbClient = app().getDbClient();
    auto callbackPtr =
        std::make_shared<std::function<void(const HttpResponsePtr &)>>(
            std::move(callback));

    auto foundCandidate = std::make_shared<bool>(false);
    auto foundVacancy = std::make_shared<bool>(false);
    auto alreadyResponded = std::make_shared<bool>(false);
    auto finishedChecks = std::make_shared<int>(0);

    auto checkComplete = [=]() {
        (*finishedChecks)++;
        if (*finishedChecks == 3) {
            if (!*foundCandidate) {
                Json::Value err;
                err["error"] = "Candidate not found";
                const auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k404NotFound);
                (*callbackPtr)(resp);
            } else if (!*foundVacancy) {
                Json::Value err;
                err["error"] = "Vacancy not found";
                const auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k404NotFound);
                (*callbackPtr)(resp);
            } else if (*alreadyResponded) {
                Json::Value err;
                err["error"] = "Response already exists";
                const auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k409Conflict);
                (*callbackPtr)(resp);
            } else {
                try {
                    drogon_model::default_db::Responces responce;
                    responce.setCandidateId(candidate_id);
                    responce.setVacancyId(vacancy_id);
                    responce.setEmployerId(employer_id);
                    responce.setCreatedAt(trantor::Date::now());

                    orm::Mapper<drogon_model::default_db::Responces>
                        responcesMapper(dbClient);
                    responcesMapper.insert(
                        responce,
                        [=](const drogon_model::default_db::Responces &) {
                            Json::Value ok;
                            ok["message"] = "Response recorded successfully";
                            const auto resp =
                                HttpResponse::newHttpJsonResponse(ok);
                            (*callbackPtr)(resp);
                        },
                        [=](const orm::DrogonDbException &e) {
                            Json::Value err;
                            err["error"] = "Failed to insert response";
                            err["detail"] = e.base().what();
                            const auto resp =
                                HttpResponse::newHttpJsonResponse(err);
                            resp->setStatusCode(k500InternalServerError);
                            (*callbackPtr)(resp);
                        });
                } catch (const std::exception &e) {
                    Json::Value err;
                    err["error"] = "Exception while preparing response insert";
                    err["detail"] = e.what();
                    const auto resp = HttpResponse::newHttpJsonResponse(err);
                    resp->setStatusCode(k500InternalServerError);
                    (*callbackPtr)(resp);
                }
            }
        }
    };

    try {
        orm::Mapper<drogon_model::default_db::Candidates> candidatesMapper(
            dbClient);
        candidatesMapper.findOne(
            orm::Criteria(drogon_model::default_db::Candidates::Cols::_user_id,
                          candidate_id),
            [=](const drogon_model::default_db::Candidates &) {
                *foundCandidate = true;
                checkComplete();
            },
            [=](const orm::DrogonDbException &) {
                *foundCandidate = false;
                checkComplete();
            });
    } catch (const std::exception &e) {
        Json::Value err;
        err["error"] = "Database error";
        LOG_DEBUG << "Database error (candidate check): " << e.what();
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
        return;
    }

    try {
        orm::Mapper<drogon_model::default_db::Vacancies> vacanciesMapper(
            dbClient);
        vacanciesMapper.findOne(
            orm::Criteria(drogon_model::default_db::Vacancies::Cols::_id,
                          vacancy_id),
            [=](const drogon_model::default_db::Vacancies &) {
                *foundVacancy = true;
                checkComplete();
            },
            [=](const orm::DrogonDbException &) {
                *foundVacancy = false;
                checkComplete();
            });
    } catch (const std::exception &e) {
        Json::Value err;
        err["error"] = "Database error";
        LOG_DEBUG << "Database error (vacancy check): " << e.what();
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
    }
    try {
        orm::Mapper<drogon_model::default_db::Responces> responcesMapper(
            dbClient);
        responcesMapper.findOne(
            orm::Criteria(
                drogon_model::default_db::Responces::Cols::_candidate_id,
                candidate_id) &&
                orm::Criteria(
                    drogon_model::default_db::Responces::Cols::_vacancy_id,
                    vacancy_id),
            [=](const drogon_model::default_db::Responces &) {
                *alreadyResponded = true;
                checkComplete();
            },
            [=](const orm::DrogonDbException &) {
                *alreadyResponded = false;
                checkComplete();
            });
    } catch (const std::exception &e) {
        Json::Value err;
        err["error"] = "Database error (response check)";
        err["detail"] = e.what();
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
    }
}

void Notifications::emplRespond(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto &json = req->getJsonObject();
    if (!json || !json->isMember("vacancy_id") ||
        !json->isMember("candidate_id") || !json->isMember("employer_id")) {
        Json::Value err;
        err["error"] = "Missing vacancy_id, candidate_id, or employer_id";
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    int vacancy_id = (*json)["vacancy_id"].asInt();
    int candidate_id = (*json)["candidate_id"].asInt();
    int employer_id = (*json)["employer_id"].asInt();
    std::string contact_info = json->isMember("contact_info")
                                   ? (*json)["contact_info"].asString()
                                   : "";

    const auto &dbClient = app().getDbClient();
    auto callbackPtr =
        std::make_shared<std::function<void(const HttpResponsePtr &)>>(
            std::move(callback));

    auto foundCandidate = std::make_shared<bool>(false);
    auto foundVacancy = std::make_shared<bool>(false);
    auto foundEmployer = std::make_shared<bool>(false);
    auto vacancyMatchesEmployer = std::make_shared<bool>(false);

    auto finishedChecks = std::make_shared<int>(0);

    auto checkComplete = [=]() {
        (*finishedChecks)++;
        if (*finishedChecks == 4) {
            if (!*foundCandidate || !*foundVacancy || !*foundEmployer) {
                Json::Value err;
                if (!*foundCandidate)
                    err["error"] = "Candidate not found";
                else if (!*foundVacancy)
                    err["error"] = "Vacancy not found";
                else
                    err["error"] = "Employer not found";
                const auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k404NotFound);
                (*callbackPtr)(resp);
                return;
            }

            if (!*vacancyMatchesEmployer) {
                Json::Value err;
                err["error"] = "Vacancy does not belong to specified employer";
                const auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k403Forbidden);
                (*callbackPtr)(resp);
                return;
            }

            // Проверка: есть ли уже такая запись в Invitations
            try {
                orm::Mapper<drogon_model::default_db::Invitations>
                    invitationMapper(dbClient);
                invitationMapper.findOne(
                    orm::Criteria(drogon_model::default_db::Invitations::Cols::
                                      _vacancy_id,
                                  vacancy_id) &&
                        orm::Criteria(drogon_model::default_db::Invitations::
                                          Cols::_candidate_id,
                                      candidate_id),
                    [=](const drogon_model::default_db::Invitations &) {
                        Json::Value err;
                        err["error"] = "Invitation already exists";
                        const auto resp =
                            HttpResponse::newHttpJsonResponse(err);
                        resp->setStatusCode(k400BadRequest);
                        (*callbackPtr)(resp);
                    },
                    // Если не найдено — вставим
                    [=](const orm::DrogonDbException &) {
                        // Если контактная информация не указана — подгружаем из
                        // Employers
                        auto insertInvitation = [=](const std::string
                                                        &resolvedContact) {
                            drogon_model::default_db::Invitations invitation;
                            invitation.setVacancyId(vacancy_id);
                            invitation.setCandidateId(candidate_id);
                            invitation.setContactInfo(resolvedContact);
                            invitation.setCreatedAt(trantor::Date::now());

                            orm::Mapper<drogon_model::default_db::Invitations>
                                mapper(dbClient);
                            mapper.insert(
                                invitation,
                                [=](const drogon_model::default_db::Invitations
                                        &) {
                                    Json::Value ok;
                                    ok["message"] =
                                        "Invitation sent successfully";
                                    const auto resp =
                                        HttpResponse::newHttpJsonResponse(ok);
                                    (*callbackPtr)(resp);
                                },
                                [=](const orm::DrogonDbException &e) {
                                    Json::Value err;
                                    err["error"] =
                                        "Failed to insert invitation";
                                    err["detail"] = e.base().what();
                                    const auto resp =
                                        HttpResponse::newHttpJsonResponse(err);
                                    resp->setStatusCode(
                                        k500InternalServerError);
                                    (*callbackPtr)(resp);
                                });
                        };

                        if (!contact_info.empty()) {
                            insertInvitation(contact_info);
                        } else {
                            // Получаем email из Employers
                            orm::Mapper<drogon_model::default_db::Employers>
                                employerMapper(dbClient);
                            employerMapper.findOne(
                                orm::Criteria(drogon_model::default_db::
                                                  Employers::Cols::_user_id,
                                              employer_id),
                                [=](const drogon_model::default_db::Employers
                                        &emp) {
                                    insertInvitation(emp.getValueOfEmail());
                                },
                                [=](const orm::DrogonDbException &e) {
                                    Json::Value err;
                                    err["error"] = "Contact info not provided "
                                                   "and employer not found";
                                    err["detail"] = e.base().what();
                                    const auto resp =
                                        HttpResponse::newHttpJsonResponse(err);
                                    resp->setStatusCode(
                                        k500InternalServerError);
                                    (*callbackPtr)(resp);
                                });
                        }
                    });
            } catch (const std::exception &e) {
                Json::Value err;
                err["error"] = "Database error during invitation check";
                err["detail"] = e.what();
                const auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k500InternalServerError);
                (*callbackPtr)(resp);
            }
        }
    };

    // Проверка существования кандидата
    try {
        orm::Mapper<drogon_model::default_db::Candidates> candidatesMapper(
            dbClient);
        candidatesMapper.findOne(
            orm::Criteria(drogon_model::default_db::Candidates::Cols::_user_id,
                          candidate_id),
            [=](const drogon_model::default_db::Candidates &) {
                *foundCandidate = true;
                checkComplete();
            },
            [=](const orm::DrogonDbException &) {
                *foundCandidate = false;
                checkComplete();
            });
    } catch (...) {
        Json::Value err;
        err["error"] = "Candidate DB error";
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
        return;
    }

    // Проверка вакансии
    try {
        orm::Mapper<drogon_model::default_db::Vacancies> vacanciesMapper(
            dbClient);
        vacanciesMapper.findOne(
            orm::Criteria(drogon_model::default_db::Vacancies::Cols::_id,
                          vacancy_id),
            [=](const drogon_model::default_db::Vacancies &) {
                *foundVacancy = true;
                checkComplete();
            },
            [=](const orm::DrogonDbException &) {
                *foundVacancy = false;
                checkComplete();
            });
    } catch (...) {
        Json::Value err;
        err["error"] = "Vacancy DB error";
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
        return;
    }

    // Проверка работодателя
    try {
        orm::Mapper<drogon_model::default_db::Employers> employersMapper(
            dbClient);
        employersMapper.findOne(
            orm::Criteria(drogon_model::default_db::Employers::Cols::_user_id,
                          employer_id),
            [=](const drogon_model::default_db::Employers &) {
                *foundEmployer = true;
                checkComplete();
            },
            [=](const orm::DrogonDbException &) {
                *foundEmployer = false;
                checkComplete();
            });
    } catch (...) {
        Json::Value err;
        err["error"] = "Employer DB error";
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
        return;
    }

    try {
        orm::Mapper<drogon_model::default_db::Vacancies> vacanciesMapper(
            dbClient);
        vacanciesMapper.findOne(
            orm::Criteria(drogon_model::default_db::Vacancies::Cols::_id,
                          vacancy_id),
            [=](const drogon_model::default_db::Vacancies &vac) {
                *foundVacancy = true;
                *vacancyMatchesEmployer =
                    (vac.getValueOfEmployerId() == employer_id);
                checkComplete();
            },
            [=](const orm::DrogonDbException &) {
                *foundVacancy = false;
                *vacancyMatchesEmployer = false;
                checkComplete();
            });
    } catch (...) {
        Json::Value err;
        err["error"] = "Vacancy DB error";
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
        return;
    }
}

void Notifications::getResponsesForEmpl(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback)
{
    const auto &json = req->getJsonObject();
    const auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));

    if (!json || !json->isMember("employer_id")) {
        Json::Value err;
        err["error"] = "Missing employer_id";
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k400BadRequest);
        (*callbackPtr)(resp);
        return;
    }

    int employer_id = (*json)["employer_id"].asInt();
    const auto &dbClient = app().getDbClient();

    try {
        orm::Mapper<drogon_model::default_db::Responces> responseMapper(dbClient);
        responseMapper.findBy(
            orm::Criteria(drogon_model::default_db::Responces::Cols::_employer_id, employer_id) &&
            orm::Criteria(drogon_model::default_db::Responces::Cols::_status, "Active"),
            [callbackPtr](const std::vector<drogon_model::default_db::Responces> &responses) {
                Json::Value result;
                result["responses"] = Json::arrayValue;

                for (const auto &r : responses) {
                    Json::Value rJson;
                    rJson["vacancy_id"] = r.getValueOfVacancyId();
                    rJson["candidate_id"] = r.getValueOfCandidateId();
                    rJson["created_at"] = r.getValueOfCreatedAt().toFormattedString(false);
                    rJson["status"] = r.getValueOfStatus();
                    result["responses"].append(rJson);
                }

                auto resp = HttpResponse::newHttpJsonResponse(result);
                (*callbackPtr)(resp);
            },
            [callbackPtr](const orm::DrogonDbException &e) {
                Json::Value err;
                err["error"] = "Failed to fetch responses";
                err["detail"] = e.base().what();
                auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k500InternalServerError);
                (*callbackPtr)(resp);
            });
    } catch (const std::exception &e) {
        Json::Value err;
        err["error"] = "Exception occurred";
        err["detail"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
    }
}



void Notifications::getInvitationsForCand(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {

    const auto &json = req->getJsonObject();
    const auto callbackPtr = std::make_shared<std::function<void(const HttpResponsePtr &)>>(std::move(callback));

    if (!json || !json->isMember("candidate_id")) {
        Json::Value err;
        err["error"] = "Missing candidate_id";
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k400BadRequest);
        (*callbackPtr)(resp);
        return;
    }

    const int candidate_id = (*json)["candidate_id"].asInt();
    const auto &dbClient = app().getDbClient();

    try {
        orm::Mapper<drogon_model::default_db::Invitations> invitationsMapper(dbClient);
        invitationsMapper.findBy(
            orm::Criteria(drogon_model::default_db::Invitations::Cols::_candidate_id, candidate_id) &&
            orm::Criteria(drogon_model::default_db::Invitations::Cols::_status, "Active"),
            [callbackPtr](const std::vector<drogon_model::default_db::Invitations> &invitations) {
                Json::Value result;
                for (const auto &inv : invitations) {
                    Json::Value item;
                    item["vacancy_id"] = inv.getValueOfVacancyId();
                    item["contact_info"] = inv.getValueOfContactInfo();
                    item["created_at"] = inv.getValueOfCreatedAt().toFormattedString(false);
                    item["status"] = inv.getValueOfStatus();
                    result["invitations"].append(item);
                }
                const auto resp = HttpResponse::newHttpJsonResponse(result);
                (*callbackPtr)(resp);
            },
            [callbackPtr](const orm::DrogonDbException &e) {
                Json::Value err;
                err["error"] = "Database query failed";
                err["detail"] = e.base().what();
                const auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k500InternalServerError);
                (*callbackPtr)(resp);
            });
    } catch (const std::exception &e) {
        Json::Value err;
        err["error"] = "Exception during DB operation";
        err["detail"] = e.what();
        const auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        (*callbackPtr)(resp);
    }
}