#pragma once

#ifndef _GET_COST_DETAIL_DO_H_
#define _GET_COST_DETAIL_DO_H_

#include "../DoInclude.h"

// 患者本次住院费用汇总只读投影
// 数据由 adm_patient、adm_encounter 和 adm_charge_item 联合查询得到，因此不绑定单张数据库表
class GetCostDetailDO : public BaseDO {
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	MYSQL_SYNTHESIZE(std::string, patientName, PatientName);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, encounterBusNo, EncounterBusNo);
	MYSQL_SYNTHESIZE(std::string, totalAmount, TotalAmount);
	MYSQL_SYNTHESIZE(std::string, settledAmount, SettledAmount);
	MYSQL_SYNTHESIZE(std::string, unsettledAmount, UnsettledAmount);

public:
	GetCostDetailDO() : BaseDO("") {
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("patient_name", "s", patientName);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("encounter_bus_no", "s", encounterBusNo);
		MYSQL_ADD_FIELD("total_amount", "s", totalAmount);
		MYSQL_ADD_FIELD("settled_amount", "s", settledAmount);
		MYSQL_ADD_FIELD("unsettled_amount", "s", unsettledAmount);
	}
};

typedef std::shared_ptr<GetCostDetailDO> PtrGetCostDetailDO;

// 单条住院费用明细只读投影
// 数据由 adm_charge_item、adm_charge_item_definition、adm_organization 和 adm_practitioner 联合查询得到
class CostDetailItemDO : public BaseDO {
	MYSQL_SYNTHESIZE(std::string, chargeItemId, ChargeItemId);
	MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
	MYSQL_SYNTHESIZE(std::string, prescriptionNo, PrescriptionNo);
	MYSQL_SYNTHESIZE(std::string, itemName, ItemName);
	MYSQL_SYNTHESIZE(std::string, chargeCategoryCode, ChargeCategoryCode);
	MYSQL_SYNTHESIZE(std::string, chargeCategoryName, ChargeCategoryName);
	MYSQL_SYNTHESIZE(std::string, quantityValue, QuantityValue);
	MYSQL_SYNTHESIZE(std::string, quantityUnit, QuantityUnit);
	MYSQL_SYNTHESIZE(std::string, unitPrice, UnitPrice);
	MYSQL_SYNTHESIZE(std::string, totalPrice, TotalPrice);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(std::string, occurrenceTime, OccurrenceTime);
	MYSQL_SYNTHESIZE(std::string, performingOrgName, PerformingOrgName);
	MYSQL_SYNTHESIZE(std::string, entererName, EntererName);

public:
	CostDetailItemDO() : BaseDO("") {
		MYSQL_ADD_FIELD("charge_item_id", "s", chargeItemId);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
		MYSQL_ADD_FIELD("item_name", "s", itemName);
		MYSQL_ADD_FIELD("charge_category_code", "s", chargeCategoryCode);
		MYSQL_ADD_FIELD("charge_category_name", "s", chargeCategoryName);
		MYSQL_ADD_FIELD("quantity_value", "s", quantityValue);
		MYSQL_ADD_FIELD("quantity_unit", "s", quantityUnit);
		MYSQL_ADD_FIELD("unit_price", "s", unitPrice);
		MYSQL_ADD_FIELD("total_price", "s", totalPrice);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("occurrence_time", "s", occurrenceTime);
		MYSQL_ADD_FIELD("performing_org_name", "s", performingOrgName);
		MYSQL_ADD_FIELD("enterer_name", "s", entererName);
	}
};

typedef std::shared_ptr<CostDetailItemDO> PtrCostDetailItemDO;

#endif
