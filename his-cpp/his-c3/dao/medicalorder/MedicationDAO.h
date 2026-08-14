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

	// 按医嘱ID和租户、就诊、患者范围查询，避免跨患者或跨就诊操作。
    PtrMedicationRequestDO selectByScope(const std::string& id, const std::string& tenantId, const std::string& encounterId, const std::string& patientId);
	// 按原状态更新医嘱，只有实际更新一条记录才算操作成功。
    int updateStatus(const std::string& id, const std::string& tenantId, const std::string& encounterId, const std::string& patientId, const std::string& expectedStatus, const std::string& targetStatus, const std::string& operatorId);
};

#endif // !_MEDICATION_DAO_
