#pragma once
#ifndef _MEDICATIONREQUESTDAO_H_
#define _MEDICATIONREQUESTDAO_H_

#include "BaseDAO.h"
#include "domain/do/prescription2/MedicationRequestDO.h"

// med_medication_request 记录的数据访问对象。
class MedicationRequestDAO : public BaseDAO
{
public:
    // 查询未删除的医嘱记录。
    PtrMedicationRequestDO selectById(const std::string& id);
    // 查询并锁定用于状态流转的状态字段。
    PtrMedicationRequestDO selectByIdForUpdate(const std::string& id);
};

#endif // !_MEDICATIONREQUESTDAO_H_
