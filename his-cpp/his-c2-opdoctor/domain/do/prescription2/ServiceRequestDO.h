#pragma once

#ifndef _SERVICEREQUESTDO_H_
#define _SERVICEREQUESTDO_H_

#include "../DoInclude.h"

class ServiceRequestDO : public BaseDO
{
    // 服务申请单标识
    MYSQL_SYNTHESIZE(std::string, id, Id);
    // 处方编号
    MYSQL_SYNTHESIZE(std::string, prescriptionNo, PrescriptionNo);
    // 就诊标识
    MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
    // 患者标识
    MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
    // 乐观锁版本号
    MYSQL_SYNTHESIZE(int, version, Version);
    // 服务申请单状态
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    // 服务申请单类别
    MYSQL_SYNTHESIZE(std::string, categoryEnum, CategoryEnum);
    // 申请数量
    MYSQL_SYNTHESIZE(double, quantity, Quantity);
    // 数量单位编码
    MYSQL_SYNTHESIZE(std::string, unitCode, UnitCode);
    // 申请创建时间
    MYSQL_SYNTHESIZE(std::string, authoredTime, AuthoredTime);
    // 申请医生标识
    MYSQL_SYNTHESIZE(std::string, requesterId, RequesterId);
    // 执行医生标识
    MYSQL_SYNTHESIZE(std::string, performerId, PerformerId);
    // 执行地点标识
    MYSQL_SYNTHESIZE(std::string, locationId, LocationId);
    // 机构标识
    MYSQL_SYNTHESIZE(std::string, orgId, OrgId);
    // 申请备注
    MYSQL_SYNTHESIZE(std::string, remark, Remark);
    // 已完成打印次数
    MYSQL_SYNTHESIZE(int, printCount, PrintCount);

    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    // 最后更新时间
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);

public:
    // 将字段绑定到 wkf_service_request 表
    ServiceRequestDO() : BaseDO("wkf_service_request")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("version", "i", version);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
        MYSQL_ADD_FIELD("quantity", "d", quantity);
        MYSQL_ADD_FIELD("unit_code", "s", unitCode);
        MYSQL_ADD_FIELD("authored_time", "s", authoredTime);
        MYSQL_ADD_FIELD("requester_id", "s", requesterId);
        MYSQL_ADD_FIELD("performer_id", "s", performerId);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
        MYSQL_ADD_FIELD("remark", "s", remark);
        MYSQL_ADD_FIELD("print_count", "i", printCount);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
    }
};

typedef std::shared_ptr<ServiceRequestDO> PtrServiceRequestDO;

#endif // !_SERVICEREQUESTDO_H_
