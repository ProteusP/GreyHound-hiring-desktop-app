/**
 *
 *  CandidatesSkills.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "CandidatesSkills.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::default_db;

const std::string CandidatesSkills::Cols::_candidate_id = "candidate_id";
const std::string CandidatesSkills::Cols::_skill_id = "skill_id";
const std::vector<std::string> CandidatesSkills::primaryKeyName = {
    "candidate_id", "skill_id"};
const bool CandidatesSkills::hasPrimaryKey = true;
const std::string CandidatesSkills::tableName = "candidates_skills";

const std::vector<typename CandidatesSkills::MetaData>
    CandidatesSkills::metaData_ = {
        {"candidate_id", "int32_t", "int", 4, 0, 1, 1},
        {"skill_id", "int32_t", "int", 4, 0, 1, 1}};
const std::string &
CandidatesSkills::getColumnName(size_t index) noexcept(false) {
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
CandidatesSkills::CandidatesSkills(const Row &r,
                                   const ssize_t indexOffset) noexcept {
    if (indexOffset < 0) {
        if (!r["candidate_id"].isNull()) {
            candidateId_ =
                std::make_shared<int32_t>(r["candidate_id"].as<int32_t>());
        }
        if (!r["skill_id"].isNull()) {
            skillId_ = std::make_shared<int32_t>(r["skill_id"].as<int32_t>());
        }
    } else {
        size_t offset = (size_t)indexOffset;
        if (offset + 2 > r.size()) {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if (!r[index].isNull()) {
            candidateId_ = std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 1;
        if (!r[index].isNull()) {
            skillId_ = std::make_shared<int32_t>(r[index].as<int32_t>());
        }
    }
}

CandidatesSkills::CandidatesSkills(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 2) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if (!pMasqueradingVector[0].empty() &&
        pJson.isMember(pMasqueradingVector[0])) {
        dirtyFlag_[0] = true;
        if (!pJson[pMasqueradingVector[0]].isNull()) {
            candidateId_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if (!pMasqueradingVector[1].empty() &&
        pJson.isMember(pMasqueradingVector[1])) {
        dirtyFlag_[1] = true;
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            skillId_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[1]].asInt64());
        }
    }
}

CandidatesSkills::CandidatesSkills(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("candidate_id")) {
        dirtyFlag_[0] = true;
        if (!pJson["candidate_id"].isNull()) {
            candidateId_ = std::make_shared<int32_t>(
                (int32_t)pJson["candidate_id"].asInt64());
        }
    }
    if (pJson.isMember("skill_id")) {
        dirtyFlag_[1] = true;
        if (!pJson["skill_id"].isNull()) {
            skillId_ =
                std::make_shared<int32_t>((int32_t)pJson["skill_id"].asInt64());
        }
    }
}

void CandidatesSkills::updateByMasqueradedJson(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector) noexcept(false) {
    if (pMasqueradingVector.size() != 2) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if (!pMasqueradingVector[0].empty() &&
        pJson.isMember(pMasqueradingVector[0])) {
        if (!pJson[pMasqueradingVector[0]].isNull()) {
            candidateId_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[0]].asInt64());
        }
    }
    if (!pMasqueradingVector[1].empty() &&
        pJson.isMember(pMasqueradingVector[1])) {
        if (!pJson[pMasqueradingVector[1]].isNull()) {
            skillId_ = std::make_shared<int32_t>(
                (int32_t)pJson[pMasqueradingVector[1]].asInt64());
        }
    }
}

void CandidatesSkills::updateByJson(const Json::Value &pJson) noexcept(false) {
    if (pJson.isMember("candidate_id")) {
        if (!pJson["candidate_id"].isNull()) {
            candidateId_ = std::make_shared<int32_t>(
                (int32_t)pJson["candidate_id"].asInt64());
        }
    }
    if (pJson.isMember("skill_id")) {
        if (!pJson["skill_id"].isNull()) {
            skillId_ =
                std::make_shared<int32_t>((int32_t)pJson["skill_id"].asInt64());
        }
    }
}

const int32_t &CandidatesSkills::getValueOfCandidateId() const noexcept {
    static const int32_t defaultValue = int32_t();
    if (candidateId_)
        return *candidateId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &
CandidatesSkills::getCandidateId() const noexcept {
    return candidateId_;
}
void CandidatesSkills::setCandidateId(const int32_t &pCandidateId) noexcept {
    candidateId_ = std::make_shared<int32_t>(pCandidateId);
    dirtyFlag_[0] = true;
}

const int32_t &CandidatesSkills::getValueOfSkillId() const noexcept {
    static const int32_t defaultValue = int32_t();
    if (skillId_)
        return *skillId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &CandidatesSkills::getSkillId() const noexcept {
    return skillId_;
}
void CandidatesSkills::setSkillId(const int32_t &pSkillId) noexcept {
    skillId_ = std::make_shared<int32_t>(pSkillId);
    dirtyFlag_[1] = true;
}

void CandidatesSkills::updateId(const uint64_t id) {}
typename CandidatesSkills::PrimaryKeyType
CandidatesSkills::getPrimaryKey() const {
    return std::make_tuple(*candidateId_, *skillId_);
}

const std::vector<std::string> &CandidatesSkills::insertColumns() noexcept {
    static const std::vector<std::string> inCols = {"candidate_id", "skill_id"};
    return inCols;
}

void CandidatesSkills::outputArgs(
    drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getCandidateId()) {
            binder << getValueOfCandidateId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getSkillId()) {
            binder << getValueOfSkillId();
        } else {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> CandidatesSkills::updateColumns() const {
    std::vector<std::string> ret;
    if (dirtyFlag_[0]) {
        ret.push_back(getColumnName(0));
    }
    if (dirtyFlag_[1]) {
        ret.push_back(getColumnName(1));
    }
    return ret;
}

void CandidatesSkills::updateArgs(
    drogon::orm::internal::SqlBinder &binder) const {
    if (dirtyFlag_[0]) {
        if (getCandidateId()) {
            binder << getValueOfCandidateId();
        } else {
            binder << nullptr;
        }
    }
    if (dirtyFlag_[1]) {
        if (getSkillId()) {
            binder << getValueOfSkillId();
        } else {
            binder << nullptr;
        }
    }
}
Json::Value CandidatesSkills::toJson() const {
    Json::Value ret;
    if (getCandidateId()) {
        ret["candidate_id"] = getValueOfCandidateId();
    } else {
        ret["candidate_id"] = Json::Value();
    }
    if (getSkillId()) {
        ret["skill_id"] = getValueOfSkillId();
    } else {
        ret["skill_id"] = Json::Value();
    }
    return ret;
}

Json::Value CandidatesSkills::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const {
    Json::Value ret;
    if (pMasqueradingVector.size() == 2) {
        if (!pMasqueradingVector[0].empty()) {
            if (getCandidateId()) {
                ret[pMasqueradingVector[0]] = getValueOfCandidateId();
            } else {
                ret[pMasqueradingVector[0]] = Json::Value();
            }
        }
        if (!pMasqueradingVector[1].empty()) {
            if (getSkillId()) {
                ret[pMasqueradingVector[1]] = getValueOfSkillId();
            } else {
                ret[pMasqueradingVector[1]] = Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if (getCandidateId()) {
        ret["candidate_id"] = getValueOfCandidateId();
    } else {
        ret["candidate_id"] = Json::Value();
    }
    if (getSkillId()) {
        ret["skill_id"] = getValueOfSkillId();
    } else {
        ret["skill_id"] = Json::Value();
    }
    return ret;
}

bool CandidatesSkills::validateJsonForCreation(const Json::Value &pJson,
                                               std::string &err) {
    if (pJson.isMember("candidate_id")) {
        if (!validJsonOfField(0, "candidate_id", pJson["candidate_id"], err,
                              true))
            return false;
    } else {
        err = "The candidate_id column cannot be null";
        return false;
    }
    if (pJson.isMember("skill_id")) {
        if (!validJsonOfField(1, "skill_id", pJson["skill_id"], err, true))
            return false;
    } else {
        err = "The skill_id column cannot be null";
        return false;
    }
    return true;
}
bool CandidatesSkills::validateMasqueradedJsonForCreation(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector, std::string &err) {
    if (pMasqueradingVector.size() != 2) {
        err = "Bad masquerading vector";
        return false;
    }
    try {
        if (!pMasqueradingVector[0].empty()) {
            if (pJson.isMember(pMasqueradingVector[0])) {
                if (!validJsonOfField(0, pMasqueradingVector[0],
                                      pJson[pMasqueradingVector[0]], err, true))
                    return false;
            } else {
                err =
                    "The " + pMasqueradingVector[0] + " column cannot be null";
                return false;
            }
        }
        if (!pMasqueradingVector[1].empty()) {
            if (pJson.isMember(pMasqueradingVector[1])) {
                if (!validJsonOfField(1, pMasqueradingVector[1],
                                      pJson[pMasqueradingVector[1]], err, true))
                    return false;
            } else {
                err =
                    "The " + pMasqueradingVector[1] + " column cannot be null";
                return false;
            }
        }
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool CandidatesSkills::validateJsonForUpdate(const Json::Value &pJson,
                                             std::string &err) {
    if (pJson.isMember("candidate_id")) {
        if (!validJsonOfField(0, "candidate_id", pJson["candidate_id"], err,
                              false))
            return false;
    } else {
        err = "The value of primary key must be set in the json object for "
              "update";
        return false;
    }
    if (pJson.isMember("skill_id")) {
        if (!validJsonOfField(1, "skill_id", pJson["skill_id"], err, false))
            return false;
    } else {
        err = "The value of primary key must be set in the json object for "
              "update";
        return false;
    }
    return true;
}
bool CandidatesSkills::validateMasqueradedJsonForUpdate(
    const Json::Value &pJson,
    const std::vector<std::string> &pMasqueradingVector, std::string &err) {
    if (pMasqueradingVector.size() != 2) {
        err = "Bad masquerading vector";
        return false;
    }
    try {
        if (!pMasqueradingVector[0].empty() &&
            pJson.isMember(pMasqueradingVector[0])) {
            if (!validJsonOfField(0, pMasqueradingVector[0],
                                  pJson[pMasqueradingVector[0]], err, false))
                return false;
        } else {
            err = "The value of primary key must be set in the json object for "
                  "update";
            return false;
        }
        if (!pMasqueradingVector[1].empty() &&
            pJson.isMember(pMasqueradingVector[1])) {
            if (!validJsonOfField(1, pMasqueradingVector[1],
                                  pJson[pMasqueradingVector[1]], err, false))
                return false;
        } else {
            err = "The value of primary key must be set in the json object for "
                  "update";
            return false;
        }
    } catch (const Json::LogicError &e) {
        err = e.what();
        return false;
    }
    return true;
}
bool CandidatesSkills::validJsonOfField(size_t index,
                                        const std::string &fieldName,
                                        const Json::Value &pJson,
                                        std::string &err, bool isForCreation) {
    switch (index) {
    case 0:
        if (pJson.isNull()) {
            err = "The " + fieldName + " column cannot be null";
            return false;
        }
        if (!pJson.isInt()) {
            err = "Type error in the " + fieldName + " field";
            return false;
        }
        break;
    case 1:
        if (pJson.isNull()) {
            err = "The " + fieldName + " column cannot be null";
            return false;
        }
        if (!pJson.isInt()) {
            err = "Type error in the " + fieldName + " field";
            return false;
        }
        break;
    default:
        err = "Internal error in the server";
        return false;
    }
    return true;
}
