#include "stdafx.h"
#include <sstream>
#include "costInfoDAO.h"
#include "costInfoMapper.h"

#define MAIN_SQL_FROM \
	" aci.`id` AS id," \
	" ae.`bus_no` AS clinicNo," \
	" a_pa.`name` AS name," \
	" al.`name` AS room," \
	" ae.`start_time` AS date," \
	" tc.`display` AS feeClass," \
	" COALESCE(mmd.`category_code`, add.`category_code`, wad.`category_code`) AS detailClass," \
	" acid.`charge_name` AS projectName," \
	" COALESCE(mmd.`bus_no`, add.`bus_no`, wad.`bus_no`) AS hisID," \
	" COALESCE(mmd.`total_volume`, add.`total_volume`, NULL) AS spec," \
	" aci.`quantity_value` AS number," \
	" aci.`quantity_unit` AS unit," \
	" a_pr_d.`name` AS doctor," \
	" IF(aci.`refund_id` IS NULL, 'Charge', 'Refund') AS source," \
	" COALESCE(mmd.`chrgitm_lv`, add.`chrgitm_lv`, wad.`chrgitm_lv`) AS insuranceLevel," \
	" ysr.`insutype` AS insuranceType," \
	" acid.`price_value` AS originalPrice," \
	" aci.`discount_amount` AS discountAmount," \
	" aci.`base_amount` AS receivableAmount," \
	" aci.`system_discount_price` AS discountedPrice," \
	" aci.`unit_price` AS unitPrice," \
	" aci.`total_price` AS lineAmount," \
	" ysr.`fund_pay_sumamt` AS insuranceAmount," \
	" ysr.`psn_cash_pay` AS selfPayAmount," \
	" fcs.`settled_at` AS settleTime," \
	" a_pr_c.`name` AS checker," \
	" fpr.`tendered_amount` AS actualAmount" \
	" FROM adm_charge_item aci" \
	" LEFT JOIN adm_encounter ae ON aci.encounter_id = ae.id" \
	" LEFT JOIN adm_patient a_pa ON ae.patient_id = a_pa.id" \
	" LEFT JOIN adm_encounter_location ael ON ae.id = ael.encounter_id" \
	" LEFT JOIN adm_location al ON ael.location_id = al.id" \
	" LEFT JOIN adm_charge_item_definition acid ON aci.definition_id = acid.id" \
	" LEFT JOIN ter_concept tc ON acid.type_code = tc.code AND tc.code_system_id = 236" \
	" LEFT JOIN med_medication_definition mmd ON aci.product_table = 'med_medication_definition' AND aci.product_id = mmd.id" \
	" LEFT JOIN adm_device_definition add ON aci.product_table = 'adm_device_definition' AND aci.product_id = add.id" \
	" LEFT JOIN wkf_activity_definition wad ON aci.product_table = 'wkf_activity_definition' AND aci.product_id = wad.id" \
	" LEFT JOIN adm_practitioner a_pr_d ON aci.enterer_id = a_pr_d.id" \
	" LEFT JOIN yb_clinic_fee_upload ycfu ON aci.id = ycfu.charge_item_id" \
	" LEFT JOIN yb_settlement_record ysr ON ycfu.chrg_bchno = ysr.chrg_bchno" \
	" LEFT JOIN fin_payment_reconciliation fpr ON fpr.charge_item_ids LIKE CONCAT('%', aci.id, '%')" \
	" LEFT JOIN fin_cashier_settlement fcs ON fpr.cashier_settlement_id = fcs.id" \
	" LEFT JOIN adm_practitioner a_pr_c ON fcs.cashier_id = a_pr_c.id"

inline std::string CostInfoDAO::queryConditionBuilder(const CostInfoQuery::Wrapper& query, SqlParams& params) {
	std::stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";

	if (!query->start_time || !query->end_time) {
		sqlCondition.clear();
		sqlCondition << "error";
		return sqlCondition.str();
	}

	if (query->start_time) {
		sqlCondition << " AND aci.`occurrence_time`>=?";
		SQLPARAMS_PUSH(params, "ull", uint64_t, query->start_time.getValue(0));
	}
	if (query->end_time) {
		sqlCondition << " AND aci.`occurrence_time`<=?";
		SQLPARAMS_PUSH(params, "ull", uint64_t, query->end_time.getValue(0));
	}
	if (query->name) {
		sqlCondition << " AND a_pa.`name` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->name.getValue(""));
	}
	if (query->room) {
		sqlCondition << " AND al.`id`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->room.getValue(""));
	}
	if (query->feeClass) {
		sqlCondition << " AND acid.`type_code`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->feeClass.getValue(""));
	}
	if (query->detailClass) {
		sqlCondition << " AND COALESCE(mmd.`category_code`, add.`category_code`, wad.`category_code`)=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->detailClass.getValue(""));
	}
	if (query->projectName) {
		sqlCondition << " AND acid.`charge_name` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->projectName.getValue(""));
	}
	if (query->hisID) {
		sqlCondition << " AND COALESCE(mmd.`bus_no`, add.`bus_no`, wad.`bus_no`)=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->hisID.getValue(""));
	}
	if (query->checker) {
		sqlCondition << " AND a_pr_c.`name` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->checker.getValue(""));
	}
	// feedir: 1=refund, 0=charge
	if (query->feeDir) {
		std::string feeDirVal = query->feeDir.getValue("");
		if (feeDirVal == "1") {
			sqlCondition << " AND aci.`refund_id` IS NOT NULL";
		} else if (feeDirVal == "0") {
			sqlCondition << " AND aci.`refund_id` IS NULL";
		}
	}

	return sqlCondition.str();
}

uint64_t CostInfoDAO::count(const CostInfoQuery::Wrapper& query) {
	SqlParams params;
	string sqlStr = "SELECT COUNT(*) " MAIN_SQL_FROM;
	sqlStr += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sqlStr, params);
}

std::list<CostInfoDO> CostInfoDAO::selectWithPage(const CostInfoQuery::Wrapper& query) {

	SqlParams params;
	string sqlStr = "SELECT " MAIN_SQL_FROM;
	sqlStr += queryConditionBuilder(query, params);
	sqlStr += " ORDER BY aci.`occurrence_time` DESC, aci.`id` DESC ";
	sqlStr += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	return sqlSession->executeQuery<CostInfoDO>(sqlStr, CostInfoMapper(), params);
}

std::list<CostInfoDO> CostInfoDAO::selectAll(const CostInfoQuery::Wrapper& query) {

	SqlParams params;
	string sqlStr = "SELECT " MAIN_SQL_FROM;
	sqlStr += queryConditionBuilder(query, params);
	sqlStr += " ORDER BY aci.`occurrence_time` DESC, aci.`id` DESC ";

	return sqlSession->executeQuery<CostInfoDO>(sqlStr, CostInfoMapper(), params);
}
