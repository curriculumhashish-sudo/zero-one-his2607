#pragma once

#ifndef _GET_PATIENT_QUEUE_DO_H_
#define _GET_PATIENT_QUEUE_DO_H_

#include "../DoInclude.h"

/**
 * 患者队列行数据对象
 * 对应主 JOIN + 聚合查询的结果集
 * BaseDO("") 表示这是一个只读 DO，不绑定单张数据表
 */
class GetPatientQueueDO : public BaseDO
{
    // 就诊ID
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    // 就诊流水号
    MYSQL_SYNTHESIZE(string, encounterBusNo, EncounterBusNo);
    // 就诊开始时间
    MYSQL_SYNTHESIZE(string, startTime, StartTime);

    // 患者ID（JOIN adm_patient）
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    // 患者姓名
    MYSQL_SYNTHESIZE(string, patientName, PatientName);
    // 性别编码（MALE/FEMALE）
    MYSQL_SYNTHESIZE(string, genderCode, GenderCode);
    // 出生日期，Service层以此计算 ageText
    MYSQL_SYNTHESIZE(string, birthDate, BirthDate);

    // 科室名称（JOIN adm_organization）
    MYSQL_SYNTHESIZE(string, organizationName, OrganizationName);

    // 接诊医生姓名（通过 adm_encounter_participant 关联 adm_practitioner）
    MYSQL_SYNTHESIZE(string, doctorName, DoctorName);

    // 费用总额（子查询聚合：SUM(adm_charge_item.total_price)）
    MYSQL_SYNTHESIZE(string, totalAmount, TotalAmount);
    // 已支付金额（子查询聚合：SUM(fin_payment_reconciliation.amount)）
    MYSQL_SYNTHESIZE(string, paidAmount, PaidAmount);
    // 最近一次成功支付的 payment ID
    MYSQL_SYNTHESIZE(string, paymentId, PaymentId);
    // 支付方式编码（CASH/WECHAT/ALIPAY/SCAN_PAY/BANK_CARD）
    MYSQL_SYNTHESIZE(string, paymentType, PaymentType);

public:
    GetPatientQueueDO() : BaseDO("")
    {
        // 就诊字段
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("encounter_bus_no", "s", encounterBusNo);
        MYSQL_ADD_FIELD("start_time", "s", startTime);

        // 患者字段
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("patient_name", "s", patientName);
        MYSQL_ADD_FIELD("gender_code", "s", genderCode);
        MYSQL_ADD_FIELD("birth_date", "s", birthDate);

        // 科室字段
        MYSQL_ADD_FIELD("organization_name", "s", organizationName);

        // 医生字段
        MYSQL_ADD_FIELD("doctor_name", "s", doctorName);

        // 聚合字段
        MYSQL_ADD_FIELD("total_amount", "s", totalAmount);
        MYSQL_ADD_FIELD("paid_amount", "s", paidAmount);
        MYSQL_ADD_FIELD("payment_id", "s", paymentId);
        MYSQL_ADD_FIELD("payment_type", "s", paymentType);
    }
};

typedef std::shared_ptr<GetPatientQueueDO> PtrGetPatientQueueDO;

#endif // !_GET_PATIENT_QUEUE_DO_H_
