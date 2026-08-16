#pragma once

#ifndef _PRINT_CHARGE_RECEIPT_DO_H_
#define _PRINT_CHARGE_RECEIPT_DO_H_

#include "../DoInclude.h"

/**
 * 打印模板数据对象（接口4B）
 */
class PrintChargeReceiptTemplateDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, scenarioCode, ScenarioCode);
    MYSQL_SYNTHESIZE(string, templateVersion, TemplateVersion);
    MYSQL_SYNTHESIZE(string, printJson, PrintJson);
    MYSQL_SYNTHESIZE(string, samplePrintData, SamplePrintData);

public:
    PrintChargeReceiptTemplateDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("scenario_code", "s", scenarioCode);
        MYSQL_ADD_FIELD("version", "s", templateVersion);
        MYSQL_ADD_FIELD("print_json", "s", printJson);
        MYSQL_ADD_FIELD("sample_print_data", "s", samplePrintData);
    }
};

/**
 * 支付流水聚合数据对象（接口4A）
 * 查询 payment + encounter + patient + org
 */
class PrintChargeReceiptSlipDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    MYSQL_SYNTHESIZE(string, patientName, PatientName);
    MYSQL_SYNTHESIZE(string, hospitalName, HospitalName);
    MYSQL_SYNTHESIZE(string, genderEnumText, GenderEnumText);
    MYSQL_SYNTHESIZE(string, birthDate, BirthDate);
    MYSQL_SYNTHESIZE(string, feeNatureText, FeeNatureText);
    MYSQL_SYNTHESIZE(string, totalAmountText, TotalAmountText);
    MYSQL_SYNTHESIZE(string, cashierName, CashierName);
    MYSQL_SYNTHESIZE(string, chargeTimeText, ChargeTimeText);
    MYSQL_SYNTHESIZE(string, paymentId, PaymentId);
    MYSQL_SYNTHESIZE(string, chargeItemIds, ChargeItemIds);

public:
    PrintChargeReceiptSlipDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("patient_name", "s", patientName);
        MYSQL_ADD_FIELD("hospital_name", "s", hospitalName);
        MYSQL_ADD_FIELD("gender_enum_text", "s", genderEnumText);
        MYSQL_ADD_FIELD("birth_date", "s", birthDate);
        MYSQL_ADD_FIELD("fee_nature_text", "s", feeNatureText);
        MYSQL_ADD_FIELD("total_amount_text", "s", totalAmountText);
        MYSQL_ADD_FIELD("cashier_name", "s", cashierName);
        MYSQL_ADD_FIELD("charge_time_text", "s", chargeTimeText);
        MYSQL_ADD_FIELD("payment_id", "s", paymentId);
        MYSQL_ADD_FIELD("charge_item_ids", "s", chargeItemIds);
    }
};

#endif // !_PRINT_CHARGE_RECEIPT_DO_H_
