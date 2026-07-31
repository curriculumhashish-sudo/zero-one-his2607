#pragma once
#ifndef _MEDICATIONREQUESTDO_H_
#define _MEDICATIONREQUESTDO_H_

#include "../DoInclude.h"

// 映射至 med_medication_request 表的领域对象。
class MedicationRequestDO : public BaseDO
{
    // 医嘱记录主标识。
    MYSQL_SYNTHESIZE(std::string, id, Id);
    // 当前工作流状态，例如 DRAFT 或 COMPLETED。
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    // 最近一次工作流状态变更时间。
    MYSQL_SYNTHESIZE(std::string, statusChangedTime, StatusChangedTime);
    // 签发医嘱的用户标识。
    MYSQL_SYNTHESIZE(std::string, performerCheckId, PerformerCheckId);
    // 医嘱签发时间。
    MYSQL_SYNTHESIZE(std::string, checkTime, CheckTime);
    // 最后更新记录的用户标识。
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    // 最近一次记录更新时间。
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);

public:
    MedicationRequestDO() : BaseDO("med_medication_request")
    {
        // 定义 BaseDAO 更新所使用的对象与列映射。
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("status_changed_time", "s", statusChangedTime);
        MYSQL_ADD_FIELD("performer_check_id", "s", performerCheckId);
        MYSQL_ADD_FIELD("check_time", "s", checkTime);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
    }
};

typedef std::shared_ptr<MedicationRequestDO> PtrMedicationRequestDO;

#endif // !_MEDICATIONREQUESTDO_H_
