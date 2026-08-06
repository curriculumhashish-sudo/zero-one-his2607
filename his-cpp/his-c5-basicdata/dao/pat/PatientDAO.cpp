#include "stdafx.h"
#include "PatientDAO.h"
#include "PatientMapper.h"

#include <sstream>

namespace
{
    const char* PATIENT_COLUMNS =
        "id, tenant_id, created_at, updated_at, created_by, updated_by, "
        "is_deleted, deleted_at, deleted_by, version, bus_no, name, name_json, "
        "gender_enum, birth_date, status_enum, active_flag, temp_flag, id_card, "
        "marital_status_enum, prfs_enum, phone, address, address_province, "
        "address_city, address_district, address_street, address_extend_json, "
        "nationality_code, py_str, wb_str, blood_type_abo, blood_type_rh, "
        "work_company, native_place, country_code, link_name, link_relation_code, "
        "link_telcom, link_jsons, deceased_date, deceased_reason, organization_id, "
        "merged_target_patient_id, merged_at";

    void appendAssignment(std::ostringstream& sql, bool& first, const std::string& assignment)
    {
        if (!first) sql << ", ";
        first = false;
        sql << assignment;
    }

    void appendStringPatch(const std::string& column, const oatpp::String& value,
        std::ostringstream& sql, SqlParams& params, bool& first, bool nullable)
    {
        if (!value) return;
        appendAssignment(sql, first, column + " = ?");
        if (nullable && value->empty())
            SQLPARAMS_PUSH(params, "nul", int, 1);
        else
            SQLPARAMS_PUSH(params, "s", std::string, value->c_str());
    }

    void appendIntPatch(const std::string& column, const oatpp::Int32& value,
        std::ostringstream& sql, SqlParams& params, bool& first)
    {
        if (!value) return;
        appendAssignment(sql, first, column + " = ?");
        SQLPARAMS_PUSH(params, "i", int, value.getValue(0));
    }

    void appendBooleanPatch(const std::string& column, const oatpp::Boolean& value,
        std::ostringstream& sql, SqlParams& params, bool& first)
    {
        if (value.get() == nullptr) return;
        appendAssignment(sql, first, column + " = ?");
        SQLPARAMS_PUSH(params, "i", int, value.getValue(false) ? 1 : 0);
    }
}

std::string PatientDAO::buildSelectCondition(
    const PatSelectPageQuery::Wrapper& query,
    const std::string& tenantId,
    SqlParams& params)
{
    std::string sql = " WHERE tenant_id = ? AND is_deleted = 0 AND active_flag = 1";
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    if (query->keyword && !query->keyword->empty())
    {
        sql += " AND (name LIKE ? OR bus_no LIKE ? OR id_card LIKE ?)";
        const std::string likeValue = "%" + query->keyword.getValue("") + "%";
        SQLPARAMS_PUSH(params, "s", std::string, likeValue);
        SQLPARAMS_PUSH(params, "s", std::string, likeValue);
        SQLPARAMS_PUSH(params, "s", std::string, likeValue);
    }
    return sql;
}

std::list<PtrPatientDO> PatientDAO::selectPatList(
    const PatSelectPageQuery::Wrapper& query,
    const std::string& tenantId)
{
    SqlParams params;
    std::string sql = "SELECT id, bus_no, name, gender_enum, birth_date, phone, id_card FROM adm_patient";
    sql += buildSelectCondition(query, tenantId, params);
    sql += " ORDER BY created_at DESC";

    uint64_t pageIndex = query->pageIndex.getValue(1);
    uint64_t pageSize = query->pageSize.getValue(20);
    if (pageIndex < 1) pageIndex = 1;
    if (pageSize < 1) pageSize = 20;
    sql += " LIMIT " + std::to_string(pageSize) + " OFFSET " + std::to_string((pageIndex - 1) * pageSize);
    return sqlSession->executeQuery<PtrPatientDO>(sql, PtrPatientSelectMapper(), params);
}

uint64_t PatientDAO::countPatList(
    const PatSelectPageQuery::Wrapper& query,
    const std::string& tenantId)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM adm_patient";
    sql += buildSelectCondition(query, tenantId, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::string PatientDAO::buildPatPageCondition(
    const PatPageQuery::Wrapper& query,
    const std::string& tenantId,
    SqlParams& params)
{
    std::string sql = " WHERE tenant_id = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    if (query->name && !query->name->empty())
    {
        sql += " AND name LIKE ?";
        const std::string likeValue = "%" + query->name.getValue("") + "%";
        SQLPARAMS_PUSH(params, "s", std::string, likeValue);
    }
    if (query->idCard && !query->idCard->empty())
    {
        sql += " AND id_card LIKE ?";
        const std::string likeValue = "%" + query->idCard.getValue("") + "%";
        SQLPARAMS_PUSH(params, "s", std::string, likeValue);
    }
    return sql;
}

std::list<PtrPatientDO> PatientDAO::queryPatPage(
    const PatPageQuery::Wrapper& query,
    const std::string& tenantId)
{
    SqlParams params;
    std::string sql = "SELECT id, bus_no, name, gender_enum, birth_date, phone, id_card FROM adm_patient";
    sql += buildPatPageCondition(query, tenantId, params);
    sql += " ORDER BY created_at DESC";

    uint64_t pageIndex = query->pageIndex.getValue(1);
    uint64_t pageSize = query->pageSize.getValue(10);
    if (pageIndex < 1) pageIndex = 1;
    if (pageSize < 1) pageSize = 10;
    sql += " LIMIT " + std::to_string(pageSize) + " OFFSET " + std::to_string((pageIndex - 1) * pageSize);
    return sqlSession->executeQuery<PtrPatientDO>(sql, PtrPatientPageMapper(), params);
}

uint64_t PatientDAO::countPatPage(
    const PatPageQuery::Wrapper& query,
    const std::string& tenantId)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM adm_patient";
    sql += buildPatPageCondition(query, tenantId, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::string PatientDAO::selectTenantIdByUserId(const std::string& userId)
{
    const std::string sql =
        "SELECT tenant_id FROM whale_user_login_session "
        "WHERE user_id = ? AND tenant_id IS NOT NULL "
        "ORDER BY updated_at DESC LIMIT 1";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    return sqlSession->executeQueryOne<std::string>(sql, PatientStringMapper("tenant_id"), params);
}

PtrPatientDO PatientDAO::selectById(const std::string& id, const std::string& tenantId)
{
    const std::string sql = std::string("SELECT ") + PATIENT_COLUMNS +
        " FROM adm_patient WHERE id = ? AND tenant_id = ? AND is_deleted = 0 LIMIT 1";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeQueryOne<PtrPatientDO>(sql, PatientMapper(), params);
}

PtrPatientDO PatientDAO::selectByIdForUpdate(const std::string& id)
{
    const std::string sql = std::string("SELECT ") + PATIENT_COLUMNS +
        " FROM adm_patient WHERE id = ? AND is_deleted = 0 LIMIT 1 FOR UPDATE";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    return sqlSession->executeQueryOne<PtrPatientDO>(sql, PatientMapper(), params);
}

bool PatientDAO::existsBusNo(
    const std::string& tenantId,
    const std::string& busNo,
    const std::string& excludeId)
{
    std::string sql = "SELECT COUNT(1) FROM adm_patient WHERE tenant_id = ? AND bus_no = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, busNo);
    if (!excludeId.empty())
    {
        sql += " AND id <> ?";
        SQLPARAMS_PUSH(params, "s", std::string, excludeId);
    }
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

bool PatientDAO::existsIdCard(
    const std::string& tenantId,
    const std::string& idCard,
    const std::string& excludeId)
{
    if (idCard.empty()) return false;
    std::string sql = "SELECT COUNT(1) FROM adm_patient WHERE tenant_id = ? AND id_card = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, idCard);
    if (!excludeId.empty())
    {
        sql += " AND id <> ?";
        SQLPARAMS_PUSH(params, "s", std::string, excludeId);
    }
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

bool PatientDAO::organizationExists(
    const std::string& tenantId,
    const std::string& organizationId)
{
    const std::string sql =
        "SELECT COUNT(1) FROM adm_organization "
        "WHERE id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, organizationId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

int PatientDAO::insertPatient(const PatientDO& data)
{
    return insert(data);
}

int PatientDAO::updatePatch(
    const PatientUpdateDTO& dto,
    const std::string& tenantId,
    const std::string& userId,
    int expectedVersion)
{
    std::ostringstream sql;
    sql << "UPDATE adm_patient SET ";
    SqlParams params;
    bool first = true;

    appendStringPatch("bus_no", dto.busNo, sql, params, first, false);
    appendStringPatch("name", dto.name, sql, params, first, false);
    appendStringPatch("name_json", dto.nameJson, sql, params, first, true);
    appendStringPatch("gender_enum", dto.genderEnum, sql, params, first, false);
    appendStringPatch("birth_date", dto.birthDate, sql, params, first, true);
    appendStringPatch("status_enum", dto.statusEnum, sql, params, first, false);
    appendBooleanPatch("active_flag", dto.activeFlag, sql, params, first);
    appendBooleanPatch("temp_flag", dto.tempFlag, sql, params, first);
    appendStringPatch("id_card", dto.idCard, sql, params, first, true);
    appendStringPatch("marital_status_enum", dto.maritalStatusEnum, sql, params, first, true);
    appendStringPatch("prfs_enum", dto.prfsEnum, sql, params, first, true);
    appendStringPatch("phone", dto.phone, sql, params, first, true);
    appendStringPatch("address", dto.address, sql, params, first, true);
    appendStringPatch("address_province", dto.addressProvince, sql, params, first, true);
    appendStringPatch("address_city", dto.addressCity, sql, params, first, true);
    appendStringPatch("address_district", dto.addressDistrict, sql, params, first, true);
    appendStringPatch("address_street", dto.addressStreet, sql, params, first, true);
    appendStringPatch("address_extend_json", dto.addressExtendJson, sql, params, first, true);
    appendStringPatch("nationality_code", dto.nationalityCode, sql, params, first, true);
    appendStringPatch("py_str", dto.pyStr, sql, params, first, true);
    appendStringPatch("wb_str", dto.wbStr, sql, params, first, true);
    appendIntPatch("blood_type_abo", dto.bloodTypeAbo, sql, params, first);
    appendIntPatch("blood_type_rh", dto.bloodTypeRh, sql, params, first);
    appendStringPatch("work_company", dto.workCompany, sql, params, first, true);
    appendStringPatch("native_place", dto.nativePlace, sql, params, first, true);
    appendStringPatch("country_code", dto.countryCode, sql, params, first, true);
    appendStringPatch("link_name", dto.linkName, sql, params, first, true);
    appendIntPatch("link_relation_code", dto.linkRelationCode, sql, params, first);
    appendStringPatch("link_telcom", dto.linkTelcom, sql, params, first, true);
    appendStringPatch("link_jsons", dto.linkJsons, sql, params, first, true);
    appendStringPatch("deceased_date", dto.deceasedDate, sql, params, first, true);
    appendStringPatch("deceased_reason", dto.deceasedReason, sql, params, first, true);
    appendStringPatch("organization_id", dto.organizationId, sql, params, first, false);
    appendStringPatch("merged_target_patient_id", dto.mergedTargetPatientId, sql, params, first, true);
    appendStringPatch("merged_at", dto.mergedAt, sql, params, first, true);

    appendAssignment(sql, first, "updated_at = NOW(6)");
    appendAssignment(sql, first, "updated_by = ?");
    SQLPARAMS_PUSH(params, "s", std::string, userId);
    appendAssignment(sql, first, "version = version + 1");
    sql << " WHERE id = ? AND tenant_id = ? AND version = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(params, "s", std::string, dto.id.getValue(""));
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);
    return sqlSession->executeUpdate(sql.str(), params);
}

int PatientDAO::softDeletePatient(
    const std::string& id,
    const std::string& tenantId,
    const std::string& deletedAt,
    const std::string& deletedBy)
{
    const std::string sql =
        "UPDATE adm_patient SET is_deleted = 1, deleted_at = ?, deleted_by = ?, "
        "updated_at = ?, updated_by = ?, version = version + 1 "
        "WHERE id = ? AND tenant_id = ? AND is_deleted = 0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
    SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
    SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
    SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeUpdate(sql, params);
}
