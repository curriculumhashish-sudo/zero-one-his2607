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