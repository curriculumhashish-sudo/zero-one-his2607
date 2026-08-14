#include "stdafx.h"
#include "MedicationDAO.h"
#include "MedicationMapper.h"

// 按主键查询单条药品医嘱。
// 只查状态管理需要的字段，保留原有查询接口供其他业务使用。
PtrMedicationRequestDO MedicationDAO::selectById(std::string id)
{
	std::string sql = "SELECT id, status_enum, therapy_enum FROM med_medication_request WHERE `id`=?";
	return sqlSession->executeQueryOne<PtrMedicationRequestDO>(sql, PtrMedicationMapper(), "%s", id);
}

PtrMedicationRequestDO MedicationDAO::selectByScope(const std::string& id, const std::string& tenantId,
	const std::string& encounterId, const std::string& patientId)
{
	// 医嘱必须同时属于指定租户、就诊和患者，并且不能是逻辑删除数据。
	std::string sql = "SELECT id, status_enum, therapy_enum FROM med_medication_request "
		"WHERE id=? AND tenant_id=? AND encounter_id=? AND patient_id=? AND is_deleted=0";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, id);
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	return sqlSession->executeQueryOne<PtrMedicationRequestDO>(sql, PtrMedicationMapper(), params);
}

int MedicationDAO::updateStatus(const std::string& id, const std::string& tenantId,
	const std::string& encounterId, const std::string& patientId, const std::string& expectedStatus,
	const std::string& targetStatus, const std::string& operatorId)
{
	// 把原状态放进更新条件，防止并发请求把医嘱重复流转。
	std::string sql = "UPDATE med_medication_request SET status_enum=?, status_changed_time=NOW(6), updated_by=?, updated_at=NOW(6), "
		"version=version+1 WHERE id=? AND tenant_id=? AND encounter_id=? AND patient_id=? "
		"AND status_enum=? AND is_deleted=0";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, targetStatus);
	SQLPARAMS_PUSH(params, "s", std::string, operatorId);
	SQLPARAMS_PUSH(params, "s", std::string, id);
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, expectedStatus);
	return sqlSession->executeUpdate(sql, params);
}
