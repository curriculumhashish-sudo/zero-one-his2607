#pragma once

#ifndef _PRINT_CHARGE_RECEIPT_MAPPER_H_
#define _PRINT_CHARGE_RECEIPT_MAPPER_H_

#include "Mapper.h"
#include "domain/do/charge/PrintChargeReceiptDO.h"

class PrintChargeReceiptTemplateMapper : public Mapper<PrintChargeReceiptTemplateDO>
{
public:
    PrintChargeReceiptTemplateDO mapper(ResultSet* rs) const override
    {
        PrintChargeReceiptTemplateDO data;
        data.setBusNo(rs->getString("bus_no"));
        data.setScenarioCode(rs->getString("scenario_code"));
        data.setTemplateVersion(rs->getString("version"));
        data.setPrintJson(rs->getString("print_json"));
        data.setSamplePrintData(rs->getString("sample_print_data"));
        return data;
    }
};

class PrintChargeReceiptSlipMapper : public Mapper<PrintChargeReceiptSlipDO>
{
public:
    PrintChargeReceiptSlipDO mapper(ResultSet* rs) const override
    {
        PrintChargeReceiptSlipDO data;
        data.setEncounterId(rs->getString("encounter_id"));
        data.setPatientName(rs->getString("patient_name"));
        data.setHospitalName(rs->getString("hospital_name"));
        data.setGenderEnumText(rs->getString("gender_enum_text"));
        data.setBirthDate(rs->getString("birth_date"));
        data.setFeeNatureText(rs->getString("fee_nature_text"));
        data.setTotalAmountText(rs->getString("total_amount_text"));
        data.setCashierName(rs->getString("cashier_name"));
        data.setChargeTimeText(rs->getString("charge_time_text"));
        data.setPaymentId(rs->getString("payment_id"));
        data.setChargeItemIds(rs->getString("charge_item_ids"));
        return data;
    }
};

#endif // !_PRINT_CHARGE_RECEIPT_MAPPER_H_
