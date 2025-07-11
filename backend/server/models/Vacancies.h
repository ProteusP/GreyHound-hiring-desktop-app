/**
 *
 *  Vacancies.h
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

class Vacancies {
  public:
    struct Cols {
        static const std::string _id;
        static const std::string _name;
        static const std::string _salary;
        static const std::string _status;
        static const std::string _about;
        static const std::string _place;
        static const std::string _educ_place;
        static const std::string _experience_status_id;
        static const std::string _work_schedule_status_id;
        static const std::string _educ_status_id;
        static const std::string _employer_id;
        static const std::string _remoteness_status_id;
    };

    static const int primaryKeyNumber;
    static const std::string tableName;
    static const bool hasPrimaryKey;
    static const std::string primaryKeyName;
    using PrimaryKeyType = int32_t;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column
     * names, otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select
     * all columns by an asterisk), please set the offset to -1.
     */
    explicit Vacancies(const drogon::orm::Row &r,
                       const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Vacancies(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Vacancies(
        const Json::Value &pJson,
        const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Vacancies() = default;

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

    /**  For column id  */
    /// Get the value of the column id, returns the default value if the column
    /// is null
    const int32_t &getValueOfId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getId() const noexcept;
    /// Set the value of the column id
    void setId(const int32_t &pId) noexcept;

    /**  For column name  */
    /// Get the value of the column name, returns the default value if the
    /// column is null
    const std::string &getValueOfName() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getName() const noexcept;
    /// Set the value of the column name
    void setName(const std::string &pName) noexcept;
    void setName(std::string &&pName) noexcept;

    /**  For column salary  */
    /// Get the value of the column salary, returns the default value if the
    /// column is null
    const std::string &getValueOfSalary() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getSalary() const noexcept;
    /// Set the value of the column salary
    void setSalary(const std::string &pSalary) noexcept;
    void setSalary(std::string &&pSalary) noexcept;

    /**  For column status  */
    /// Get the value of the column status, returns the default value if the
    /// column is null
    const int8_t &getValueOfStatus() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int8_t> &getStatus() const noexcept;
    /// Set the value of the column status
    void setStatus(const int8_t &pStatus) noexcept;

    /**  For column about  */
    /// Get the value of the column about, returns the default value if the
    /// column is null
    const std::string &getValueOfAbout() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getAbout() const noexcept;
    /// Set the value of the column about
    void setAbout(const std::string &pAbout) noexcept;
    void setAbout(std::string &&pAbout) noexcept;

    /**  For column place  */
    /// Get the value of the column place, returns the default value if the
    /// column is null
    const std::string &getValueOfPlace() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getPlace() const noexcept;
    /// Set the value of the column place
    void setPlace(const std::string &pPlace) noexcept;
    void setPlace(std::string &&pPlace) noexcept;

    /**  For column educ_place  */
    /// Get the value of the column educ_place, returns the default value if the
    /// column is null
    const std::string &getValueOfEducPlace() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getEducPlace() const noexcept;
    /// Set the value of the column educ_place
    void setEducPlace(const std::string &pEducPlace) noexcept;
    void setEducPlace(std::string &&pEducPlace) noexcept;

    /**  For column experience_status_id  */
    /// Get the value of the column experience_status_id, returns the default
    /// value if the column is null
    const int32_t &getValueOfExperienceStatusId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getExperienceStatusId() const noexcept;
    /// Set the value of the column experience_status_id
    void setExperienceStatusId(const int32_t &pExperienceStatusId) noexcept;
    void setExperienceStatusIdToNull() noexcept;

    /**  For column work_schedule_status_id  */
    /// Get the value of the column work_schedule_status_id, returns the default
    /// value if the column is null
    const int32_t &getValueOfWorkScheduleStatusId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getWorkScheduleStatusId() const noexcept;
    /// Set the value of the column work_schedule_status_id
    void setWorkScheduleStatusId(const int32_t &pWorkScheduleStatusId) noexcept;
    void setWorkScheduleStatusIdToNull() noexcept;

    /**  For column educ_status_id  */
    /// Get the value of the column educ_status_id, returns the default value if
    /// the column is null
    const int32_t &getValueOfEducStatusId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getEducStatusId() const noexcept;
    /// Set the value of the column educ_status_id
    void setEducStatusId(const int32_t &pEducStatusId) noexcept;
    void setEducStatusIdToNull() noexcept;

    /**  For column employer_id  */
    /// Get the value of the column employer_id, returns the default value if
    /// the column is null
    const int32_t &getValueOfEmployerId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getEmployerId() const noexcept;
    /// Set the value of the column employer_id
    void setEmployerId(const int32_t &pEmployerId) noexcept;

    /**  For column remoteness_status_id  */
    /// Get the value of the column remoteness_status_id, returns the default
    /// value if the column is null
    const int32_t &getValueOfRemotenessStatusId() const noexcept;
    /// Return a shared_ptr object pointing to the column const value, or an
    /// empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getRemotenessStatusId() const noexcept;
    /// Set the value of the column remoteness_status_id
    void setRemotenessStatusId(const int32_t &pRemotenessStatusId) noexcept;

    static size_t getColumnNumber() noexcept { return 12; }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(
        const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
  private:
    friend drogon::orm::Mapper<Vacancies>;
    friend drogon::orm::BaseBuilder<Vacancies, true, true>;
    friend drogon::orm::BaseBuilder<Vacancies, true, false>;
    friend drogon::orm::BaseBuilder<Vacancies, false, true>;
    friend drogon::orm::BaseBuilder<Vacancies, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Vacancies>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    /// For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<std::string> name_;
    std::shared_ptr<std::string> salary_;
    std::shared_ptr<int8_t> status_;
    std::shared_ptr<std::string> about_;
    std::shared_ptr<std::string> place_;
    std::shared_ptr<std::string> educPlace_;
    std::shared_ptr<int32_t> experienceStatusId_;
    std::shared_ptr<int32_t> workScheduleStatusId_;
    std::shared_ptr<int32_t> educStatusId_;
    std::shared_ptr<int32_t> employerId_;
    std::shared_ptr<int32_t> remotenessStatusId_;
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
    bool dirtyFlag_[12] = {false};

  public:
    static const std::string &sqlForFindingByPrimaryKey() {
        static const std::string sql =
            "select * from " + tableName + " where id = ?";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey() {
        static const std::string sql =
            "delete from " + tableName + " where id = ?";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const {
        std::string sql = "insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        sql += "id,";
        ++parametersCount;
        if (dirtyFlag_[1]) {
            sql += "name,";
            ++parametersCount;
        }
        if (dirtyFlag_[2]) {
            sql += "salary,";
            ++parametersCount;
        }
        if (dirtyFlag_[3]) {
            sql += "status,";
            ++parametersCount;
        }
        if (dirtyFlag_[4]) {
            sql += "about,";
            ++parametersCount;
        }
        if (dirtyFlag_[5]) {
            sql += "place,";
            ++parametersCount;
        }
        if (dirtyFlag_[6]) {
            sql += "educ_place,";
            ++parametersCount;
        }
        if (dirtyFlag_[7]) {
            sql += "experience_status_id,";
            ++parametersCount;
        }
        if (dirtyFlag_[8]) {
            sql += "work_schedule_status_id,";
            ++parametersCount;
        }
        if (dirtyFlag_[9]) {
            sql += "educ_status_id,";
            ++parametersCount;
        }
        if (dirtyFlag_[10]) {
            sql += "employer_id,";
            ++parametersCount;
        }
        if (dirtyFlag_[11]) {
            sql += "remoteness_status_id,";
            ++parametersCount;
        }
        needSelection = true;
        if (parametersCount > 0) {
            sql[sql.length() - 1] = ')';
            sql += " values (";
        } else
            sql += ") values (";

        sql += "default,";
        if (dirtyFlag_[1]) {
            sql.append("?,");
        }
        if (dirtyFlag_[2]) {
            sql.append("?,");
        }
        if (dirtyFlag_[3]) {
            sql.append("?,");
        }
        if (dirtyFlag_[4]) {
            sql.append("?,");
        }
        if (dirtyFlag_[5]) {
            sql.append("?,");
        }
        if (dirtyFlag_[6]) {
            sql.append("?,");
        }
        if (dirtyFlag_[7]) {
            sql.append("?,");
        }
        if (dirtyFlag_[8]) {
            sql.append("?,");
        }
        if (dirtyFlag_[9]) {
            sql.append("?,");
        }
        if (dirtyFlag_[10]) {
            sql.append("?,");
        }
        if (dirtyFlag_[11]) {
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
