#pragma once
#ifndef _MEDICATION_DAO_
#define _MEDICATION_DAO_

#include "BaseDAO.h"
#include "../../domain/do/medicalorder/MedicationRequestDO.h"

/**
 * 药品医嘱数据访问对象
 * 操作med_medication_request表（药品医嘱表）
 */
class MedicationDAO : public BaseDAO
{
public:
	// 按主键查询单条药品医嘱，未找到返回null
	PtrMedicationRequestDO selectById(std::string id);
};

#endif // !_MEDICATION_DAO_