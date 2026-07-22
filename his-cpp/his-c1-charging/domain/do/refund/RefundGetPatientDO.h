#pragma once

#ifndef _GETPATIENTDO_H_
#define _GETPATIENTDO_H_

#include "../DoInclude.h"

/**
 * 退费患者队列数据对象
 * 对应多表 JOIN 查询结果，不是单表映射
 */
class GetPatientDO : public BaseDO
{
    // 就诊ID
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    // 就诊流水号（如 EN2026-07-190002）
    MYSQL_SYNTHESIZE(string, encounterBusNo, EncounterBusNo);
    // 患者ID
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    // 患者姓名
    MYSQL_SYNTHESIZE(string, patientName, PatientName);
    // 性别编码（MALE / FEMALE）
    MYSQL_SYNTHESIZE(string, genderCode, GenderCode);
    // 性别显示文本（如"男性"）
    MYSQL_SYNTHESIZE(string, genderEnumText, GenderEnumText);
    // 年龄显示文本（如"20岁"）
    MYSQL_SYNTHESIZE(string, ageText, AgeText);
    // 队列标签码：PENDING-待退费 / SETTLED-已退费
    MYSQL_SYNTHESIZE(string, queueLabelCode, QueueLabelCode);
    // 收费状态摘要（如"退款中"）
    MYSQL_SYNTHESIZE(string, chargeItemStatusSummary, ChargeItemStatusSummary);
    // 科室名称
    MYSQL_SYNTHESIZE(string, organizationName, OrganizationName);
    // 医生姓名
    MYSQL_SYNTHESIZE(string, doctorName, DoctorName);
    // 就诊开始时间
    MYSQL_SYNTHESIZE(string, startTime, StartTime);
    // 总金额（可为空）
    MYSQL_SYNTHESIZE(string, totalAmount, TotalAmount);
    // 已付金额（可为空）
    MYSQL_SYNTHESIZE(string, paidAmount, PaidAmount);

public:
    // 因为是 JOIN 视图，非单表，表名传空字符串
    GetPatientDO() : BaseDO("")
    {
        // 注册主键字段（列名, 类型码, 成员变量）
        MYSQL_ADD_FIELD_PK("encounter_id", "s", encounterId);
        // 注册普通字段
        MYSQL_ADD_FIELD("encounter_bus_no", "s", encounterBusNo);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("patient_name", "s", patientName);
        MYSQL_ADD_FIELD("gender_code", "s", genderCode);
        MYSQL_ADD_FIELD("gender_enum_text", "s", genderEnumText);
        MYSQL_ADD_FIELD("age_text", "s", ageText);
        MYSQL_ADD_FIELD("queue_label_code", "s", queueLabelCode);
        MYSQL_ADD_FIELD("charge_item_status_summary", "s", chargeItemStatusSummary);
        MYSQL_ADD_FIELD("organization_name", "s", organizationName);
        MYSQL_ADD_FIELD("doctor_name", "s", doctorName);
        MYSQL_ADD_FIELD("start_time", "s", startTime);
        MYSQL_ADD_FIELD("total_amount", "s", totalAmount);
        MYSQL_ADD_FIELD("paid_amount", "s", paidAmount);
    }
};

// 定义智能指针别名
typedef std::shared_ptr<GetPatientDO> PtrGetPatientDO;



#endif // !_GETPATIENTDO_H_
