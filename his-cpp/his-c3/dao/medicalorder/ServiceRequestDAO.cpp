#include "stdafx.h"
#include "ServiceRequestDAO.h"
#include "ServiceRequestMapper.h"

// 按主键查询单条服务医嘱
// 只查id、status_enum、therapy_enum三个字段，满足医嘱状态管理需求
PtrServiceRequestDO ServiceRequestDAO::selectById(std::string id)
{
	std::string sql = "SELECT id, status_enum, therapy_enum FROM wkf_service_request WHERE `id`=?";
	return sqlSession->executeQueryOne<PtrServiceRequestDO>(sql, PtrServiceRequestMapper(), "%s", id);
}

PtrServiceRequestDO ServiceRequestDAO::selectByScope(const std::string& id, const std::string& tenantId,
	const std::string& encounterId, const std::string& patientId)
{
	// 医嘱必须同时属于指定租户、就诊和患者，并且不能是逻辑删除数据。
	std::string sql = "SELECT id, status_enum, therapy_enum FROM wkf_service_request "
		"WHERE id=? AND tenant_id=? AND encounter_id=? AND patient_id=? AND is_deleted=0";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, id);
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	return sqlSession->executeQueryOne<PtrServiceRequestDO>(sql, PtrServiceRequestMapper(), params);
}

int ServiceRequestDAO::updateStatus(const std::string& id, const std::string& tenantId,
	const std::string& encounterId, const std::string& patientId, const std::string& expectedStatus,
	const std::string& targetStatus, const std::string& operatorId)
{
	// 把原状态放进更新条件，防止并发请求把医嘱重复流转。
	std::string sql = "UPDATE wkf_service_request SET status_enum=?, updated_by=?, updated_at=NOW(6), "
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
