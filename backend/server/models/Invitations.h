/**
 *
 *  Invitations.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/BaseBuilder.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/SqlBinder.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif
#include <iostream>
#include <json/json.h>
#include <memory>
#include <stdint.h>
#include <string>
#include <string_view>
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <tuple>
#include <vector>

namespace drogon {
namespace orm {
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
} // namespace orm
} // namespace drogon
namespace drogon_model {
namespace default_db {

class Invitations {
  public:
    struct Cols {
        static const std::string _candidate_id;
        static const std::string _vacancy_id;
        static const std::string _status;
        static const std::string _created_at;
        static const std::string _contact_info;
    };

    static const int primaryKeyNumber;
    static const std::string tableName;
    static const bool hasPrimaryKey;
    static const std::vector<std::string> primaryKeyName;
    using PrimaryKeyType =
        std::tuple<int32_t, int32_t>; // candidate_id,vacancy_id
    PrimaryKeyType getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column
     * names, otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select
     * all columns by an asterisk), please set the offset to -1.
     */
    explicit Invitations(const drogon::orm::Row &r,
                         const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Invitations(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Invitations(
        const Json::Value &pJson,
        const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Invitations() = default;

    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(
        const Json::Value &pJson,
        const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson,
                                        std::string &err);
    static bool validateMasqueradedJsonForCreation(
        const Json::Value &,
        const std::vector<std::string> &pMasqueradingVector, std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson,
                                      std::string &err);
    static bool validateMasqueradedJsonForUpdate(
        const Json::Value &,
        const std::vector<std::string> &pMasqueradingVector, std::string &err);
    static bool validJsonOfField(size_t index, const std::string &fieldName,
                                 const Json::Value &pJson, std::string &err,
                                 bool isForCreation);

    /**  For column candidate_id  */
    /// Get the value of the column candidate_id, returns the default value if
    /// the column is null
    const int32_t &getValueOfCandidateId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getCandidateId() const noexcept;
    /// Set the value of the column candidate_id
    void setCandidateId(const int32_t &pCandidateId) noexcept;

    /**  For column vacancy_id  */
    /// Get the value of the column vacancy_id, returns the default value if the
    /// column is null
    const int32_t &getValueOfVacancyId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getVacancyId() const noexcept;
    /// Set the value of the column vacancy_id
    void setVacancyId(const int32_t &pVacancyId) noexcept;

    /**  For column status  */
    /// Get the value of the column status, returns the default value if the
    /// column is null
    const std::string &getValueOfStatus() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getStatus() const noexcept;
    /// Set the value of the column status
    void setStatus(const std::string &pStatus) noexcept;
    void setStatus(std::string &&pStatus) noexcept;
    void setStatusToNull() noexcept;

    /**  For column created_at  */
    /// Get the value of the column created_at, returns the default value if the
    /// column is null
    const ::trantor::Date &getValueOfCreatedAt() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getCreatedAt() const noexcept;
    /// Set the value of the column created_at
    void setCreatedAt(const ::trantor::Date &pCreatedAt) noexcept;
    void setCreatedAtToNull() noexcept;

    /**  For column contact_info  */
    /// Get the value of the column contact_info, returns the default value if
    /// the column is null
    const std::string &getValueOfContactInfo() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getContactInfo() const noexcept;
    /// Set the value of the column contact_info
    void setContactInfo(const std::string &pContactInfo) noexcept;
    void setContactInfo(std::string &&pContactInfo) noexcept;

    static size_t getColumnNumber() noexcept { return 5; }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(
        const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
  private:
    friend drogon::orm::Mapper<Invitations>;
    friend drogon::orm::BaseBuilder<Invitations, true, true>;
    friend drogon::orm::BaseBuilder<Invitations, true, false>;
    friend drogon::orm::BaseBuilder<Invitations, false, true>;
    friend drogon::orm::BaseBuilder<Invitations, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Invitations>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    /// For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<int32_t> candidateId_;
    std::shared_ptr<int32_t> vacancyId_;
    std::shared_ptr<std::string> status_;
    std::shared_ptr<::trantor::Date> createdAt_;
    std::shared_ptr<std::string> contactInfo_;
    struct MetaData {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[5] = {false};

  public:
    static const std::string &sqlForFindingByPrimaryKey() {
        static const std::string sql =
            "select * from " + tableName +
            " where candidate_id = ? and vacancy_id = ?";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey() {
        static const std::string sql =
            "delete from " + tableName +
            " where candidate_id = ? and vacancy_id = ?";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const {
        std::string sql = "insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        if (dirtyFlag_[0]) {
            sql += "candidate_id,";
            ++parametersCount;
        }
        if (dirtyFlag_[1]) {
            sql += "vacancy_id,";
            ++parametersCount;
        }
        sql += "status,";
        ++parametersCount;
        if (!dirtyFlag_[2]) {
            needSelection = true;
        }
        sql += "created_at,";
        ++parametersCount;
        if (!dirtyFlag_[3]) {
            needSelection = true;
        }
        if (dirtyFlag_[4]) {
            sql += "contact_info,";
            ++parametersCount;
        }
        if (parametersCount > 0) {
            sql[sql.length() - 1] = ')';
            sql += " values (";
        } else
            sql += ") values (";

        if (dirtyFlag_[0]) {
            sql.append("?,");
        }
        if (dirtyFlag_[1]) {
            sql.append("?,");
        }
        if (dirtyFlag_[2]) {
            sql.append("?,");

        } else {
            sql += "default,";
        }
        if (dirtyFlag_[3]) {
            sql.append("?,");

        } else {
            sql += "default,";
        }
        if (dirtyFlag_[4]) {
            sql.append("?,");
        }
        if (parametersCount > 0) {
            sql.resize(sql.length() - 1);
        }
        sql.append(1, ')');
        LOG_TRACE << sql;
        return sql;
    }
};
} // namespace default_db
} // namespace drogon_model
