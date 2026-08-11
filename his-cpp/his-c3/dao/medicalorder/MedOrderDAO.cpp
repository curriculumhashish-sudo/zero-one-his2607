#include "stdafx.h"
#include "MedOrderDAO.h"
#include "MedOrderMapper.h"


// ---- 计数 ----
uint64_t MedOrderDAO::count(const MedicalOrderQuery::Wrapper& query)
{
    std::ostringstream sql;
    sql << "SELECT COUNT(1) AS cnt FROM med_medication_request WHERE is_deleted = 0";

    SqlParams params;
    // 动态条件拼接（原项目已有逻辑）
    if (query->patientId && !query->patientId.getValue("").empty()) {
        sql << " AND patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
    }
    if (query->encounterId && !query->encounterId.getValue("").empty()) {
        sql << " AND encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
    }
    if (query->statusEnum && !query->statusEnum.getValue("").empty()) {
        sql << " AND status_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
    }
    if (query->groupId && !query->groupId.getValue("").empty()) {
        sql << " AND group_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->groupId.getValue(""));
    }

    return sqlSession->executeQueryNumerical(sql.str(), params);
}

// ----分页查询 ----
std::list<PtrMedMedicationRequestDO> MedOrderDAO::selectAll(
    const MedicalOrderQuery::Wrapper& query)
{
    std::ostringstream sql;
    sql << "SELECT id, group_id, based_on_table, based_on_id, "
        << "status_enum, status_reason, status_changed_time, "
        << "therapy_enum, effective_dose_end, "
        << "patient_id, encounter_id, medication_id, bus_no, prescription_no "
        << "FROM med_medication_request WHERE is_deleted = 0";

    SqlParams params;
    if (query->patientId && !query->patientId.getValue("").empty()) {
        sql << " AND patient_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
    }
    if (query->encounterId && !query->encounterId.getValue("").empty()) {
        sql << " AND encounter_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->encounterId.getValue(""));
    }
    if (query->statusEnum && !query->statusEnum.getValue("").empty()) {
        sql << " AND status_enum = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->statusEnum.getValue(""));
    }
    if (query->groupId && !query->groupId.getValue("").empty()) {
        sql << " AND group_id = ?";
        SQLPARAMS_PUSH(params, "s", std::string, query->groupId.getValue(""));
    }

    sql << " ORDER BY created_at DESC";

    //分页
    if (query->pageIndex && query->pageSize) {
        int offset = static_cast<int>((query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
        sql << " LIMIT " << query->pageSize.getValue(10)
            << " OFFSET " << offset;
    }

    MedOrderMapper mapper;
    return sqlSession->executeQuery<PtrMedMedicationRequestDO>(sql.str(), mapper, params);
}

//---- 按主键查询 ----
PtrMedMedicationRequestDO MedOrderDAO::selectById(const std::string& id)
{
    if (id.empty()) return nullptr;

    std::string sql =
        "SELECT id, group_id, based_on_table, based_on_id, "
        "status_enum, status_reason, status_changed_time, "
        "therapy_enum, effective_dose_end, "
        "patient_id, encounter_id, medication_id, bus_no, prescription_no "
        "FROM med_medication_request "
        "WHERE id = ? AND is_deleted = 0 LIMIT 1";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);

    MedOrderMapper mapper;
    return sqlSession->executeQueryOne<PtrMedMedicationRequestDO>(sql, mapper, params);
}

// ---- 按组号查询 ----
std::list<PtrMedMedicationRequestDO> MedOrderDAO::selectByGroupId(
    const std::string& groupId)
{
    if (groupId.empty()) return {};

    std::string sql =
        "SELECT id, group_id, based_on_table, based_on_id, "
        "status_enum, status_reason, status_changed_time, "
        "therapy_enum, effective_dose_end, "
        "patient_id, encounter_id, medication_id, bus_no, prescription_no "
        "FROM med_medication_request "
        "WHERE group_id = ? AND is_deleted = 0 "
        "ORDER BY created_at ASC";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, groupId);

    MedOrderMapper mapper;
    return sqlSession->executeQuery<PtrMedMedicationRequestDO>(sql, mapper, params);
}


//并嘱
int MedOrderDAO::updateGroupIdBatch(const std::list<std::string>& orderIds, const std::string& groupId)
{
	if (orderIds.empty())return 0;
	SqlParams params;
	std::ostringstream sql;
	sql << "UPDATE med_medication_request "
		<< "SET group_id = ?, updated_at = NOW() "
		<< "WHERE id IN (";
	SQLPARAMS_PUSH(params, "s", std::string, groupId);

	bool first = true;
	for (auto& id : orderIds) {
		if (!first) sql << ",";
		else first = false;
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, id);
	}
	sql << ") AND is_deleted = 0";

	return sqlSession->executeUpdate(sql.str(), params);
}

//拆嘱
int MedOrderDAO::clearGroupIdBatch(const std::list<std::string>& orderIds)
{
	if (orderIds.empty()) return 0;

	SqlParams params;
	std::ostringstream sql;
	sql << "UPDATE med_medication_request "
		<< "SET group_id = NULL, "
		<< "  based_on_table = 'med_medication_request', "
		<< "  updated_at = NOW() "
		<< "WHERE id IN (";
	bool first = true;
	for (auto& id : orderIds) {
		if (!first)sql << ",";
		else first = false;
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, id);
	}
	sql << ") AND is_deleted = 0";

	return sqlSession->executeUpdate(sql.str(), params);
}

// 批量停嘱
int MedOrderDAO::stopOrderBatch(const std::list<std::string>& orderIds, int stopReason, const std::string& operatorId)
{
	if (orderIds.empty()) return 0;
	SqlParams params;
	std::ostringstream sql;

	sql << "UPDATE med_medication_request "
		<< "SET status_enum = 'STOPPED', "
		<< "    status_reason = ?, "
        << "    status_changed_time = NOW(), "
		<< "    operator_id = ?, "
		<< "    effective_dose_end = NOW(), "
		<< "    updated_at = NOW() "
		<< "WHERE id IN (";
	SQLPARAMS_PUSH(params, "i", int, stopReason);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);


	bool first = true;
	for (auto& id : orderIds) {
		if (!first) sql << ",";
		else first = false;
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, id);
	}

	sql << ") AND status_enum != 'STOPPED' AND is_deleted = 0";

	return sqlSession->executeUpdate(sql.str(), params);
}