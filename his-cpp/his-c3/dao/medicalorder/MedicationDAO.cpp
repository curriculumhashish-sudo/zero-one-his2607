#include "stdafx.h"
#include "MedicationDAO.h"
#include "MedicationMapper.h"

// 按主键查询单条药品医嘱
// 只查id、status_enum、therapy_enum三个字段，满足医嘱状态管理需求
PtrMedicationRequestDO MedicationDAO::selectById(std::string id)
{
	std::string sql = "SELECT id, status_enum, therapy_enum FROM med_medication_request WHERE `id`=?";
	return sqlSession->executeQueryOne<PtrMedicationRequestDO>(sql, PtrMedicationMapper(), "%s", id);
}