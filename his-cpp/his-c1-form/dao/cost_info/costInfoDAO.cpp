#include "stdafx.h"
#include <sstream>
#include "costInfoDAO.h"
#include "costInfoMapper.h"


/*
 表缩写参考
 aci    = adm_charge_item            收费条目表
 ae     = adm_encounter              就诊记录表
 a_pa   = adm_patient                患者表
 ael    = adm_encounter_location     就诊科室关联表
 al     = adm_location               科室/地点表
 acid   = adm_charge_item_definition 收费项目定义表
 tc     = ter_concept                字典概念表(收费大类, code_system_id=236)
 mmd    = med_medication_definition  药品定义表
 add    = adm_device_definition      器材定义表
 wad    = wkf_activity_definition    诊疗项目定义表
 tc_mmd = ter_concept                药品分类字典(code_system_id=900006)
 tc_add = ter_concept                器材分类字典(code_system_id=253)
 tc_wad = ter_concept                诊疗分类字典(code_system_id=255)
 a_pr_d = adm_practitioner           医务人员表(开单医生)
 ycfu   = yb_clinic_fee_upload       医保门诊费用上传表
 ysr    = yb_settlement_record       医保结算记录表
 fpr    = fin_payment_reconciliation 支付对账/收款记录表
 fcs    = fin_cashier_settlement     收费员结算表
 a_pr_c = adm_practitioner           医务人员表(收费员)
*/

#define MAIN_SQL_COLUMNS \
	" aci.`id` AS chargeID," \
	" ae.`id` AS encounterID," \
	" fpr.`id` AS paymentID," \
	" fcs.`cashier_id` AS cashierID," \
	" a_pa.`name` AS name," \
	" al.`name` AS room," \
	" aci.`occurrence_time` AS date," \
	" tc.`display` AS feeClass," \
	" COALESCE(tc_mmd.`display`, tc_add.`display`, tc_wad.`display`) AS detailClass," \
	" acid.`charge_name` AS projectName," \
	" COALESCE(mmd.`bus_no`, `add`.`bus_no`, wad.`bus_no`) AS hisID," \
	" COALESCE(mmd.`total_volume`, `add`.`total_volume`, NULL) AS spec," \
	" aci.`quantity_value` AS number," \
	" aci.`quantity_unit` AS unit," \
	" a_pr_d.`name` AS doctor," \
	" COALESCE(mmd.`chrgitm_lv`, `add`.`chrgitm_lv`, wad.`chrgitm_lv`) AS insuranceLevel," \
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
	" fpr.`tendered_amount` AS actualAmount"

#define MAIN_SQL_JOINS \
	" FROM adm_charge_item aci" \
	" LEFT JOIN adm_encounter ae ON aci.encounter_id = ae.id" \
	" LEFT JOIN adm_patient a_pa ON ae.patient_id = a_pa.id" \
	" LEFT JOIN adm_encounter_location ael ON ae.id = ael.encounter_id" \
	" LEFT JOIN adm_location al ON ael.location_id = al.id" \
	" LEFT JOIN adm_charge_item_definition acid ON aci.definition_id = acid.id" \
	" LEFT JOIN ter_concept tc ON acid.type_code = tc.code AND tc.code_system_id = 236" \
	" LEFT JOIN med_medication_definition mmd ON aci.product_table = 'med_medication_definition' AND aci.product_id = mmd.id" \
	" LEFT JOIN adm_device_definition `add` ON aci.product_table = 'adm_device_definition' AND aci.product_id = `add`.id" \
	" LEFT JOIN wkf_activity_definition wad ON aci.product_table = 'wkf_activity_definition' AND aci.product_id = wad.id" \
	" LEFT JOIN ter_concept tc_mmd ON mmd.category_code = tc_mmd.code AND tc_mmd.code_system_id = 900006" \
	" LEFT JOIN ter_concept tc_add ON `add`.category_code = tc_add.code AND tc_add.code_system_id = 253" \
	" LEFT JOIN ter_concept tc_wad ON wad.category_code = tc_wad.code AND tc_wad.code_system_id = 255" \
	" LEFT JOIN adm_practitioner a_pr_d ON aci.enterer_id = a_pr_d.id" \
	" LEFT JOIN yb_clinic_fee_upload ycfu ON aci.id = ycfu.charge_item_id" \
	" LEFT JOIN yb_settlement_record ysr ON ycfu.chrg_bchno = ysr.chrg_bchno" \
	" LEFT JOIN fin_payment_reconciliation fpr ON fpr.charge_item_ids LIKE CONCAT('%', aci.id, '%')" \
	" LEFT JOIN fin_cashier_settlement fcs ON fpr.cashier_settlement_id = fcs.id" \
	" LEFT JOIN adm_practitioner a_pr_c ON fcs.cashier_id = a_pr_c.id"

inline std::string CostInfoDAO::queryConditionBuilder(const CostInfoQuery::Wrapper& query, SqlParams& params) {
	std::stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";

	// 检测是否有这两个必要条件，一般不会进这个分支
	if (!query->start_time || !query->end_time) {
		sqlCondition.clear();
		sqlCondition << "error";
		return sqlCondition.str();
	}


	// 获取query中的查询条件
	if (query->start_time) {
		sqlCondition << " AND aci.`occurrence_time`>=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->start_time.getValue(""));
	}
	if (query->end_time) {
		sqlCondition << " AND aci.`occurrence_time`<=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->end_time.getValue(""));
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
		sqlCondition << " AND COALESCE(mmd.`category_code`, `add`.`category_code`, wad.`category_code`)=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->detailClass.getValue(""));
	}
	if (query->projectName) {
		sqlCondition << " AND acid.`charge_name` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->projectName.getValue(""));
	}
	if (query->hisID) {
		sqlCondition << " AND COALESCE(mmd.`bus_no`, `add`.`bus_no`, wad.`bus_no`)=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->hisID.getValue(""));
	}
	if (query->checker) {
		sqlCondition << " AND a_pr_c.`name` LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->checker.getValue(""));
	}
	// feedir: 1=refund, 0=charge（收费退费）
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
	string sqlStr = "SELECT COUNT(*)" MAIN_SQL_JOINS;
	sqlStr += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sqlStr, params);
}

std::list<CostInfoDO> CostInfoDAO::selectWithPage(const CostInfoQuery::Wrapper& query) {

	SqlParams params;
	string charge = ZH_WORDS_GETTER("rhqm.form.cost_info.other.charge");
	string refund = ZH_WORDS_GETTER("rhqm.form.cost_info.other.refund");

	string sqlStr = "SELECT DISTINCT" MAIN_SQL_COLUMNS;
	// 收费退费为了中文表示需要拆开sql
	sqlStr += ", IF(aci.`refund_id` IS NULL, '" + charge + "', '" + refund + "') AS source";
	sqlStr += MAIN_SQL_JOINS;
	sqlStr += queryConditionBuilder(query, params);
	sqlStr += " ORDER BY aci.`occurrence_time` DESC, aci.`id` DESC ";
	sqlStr += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	return sqlSession->executeQuery<CostInfoDO>(sqlStr, CostInfoMapper(), params);
}

std::list<CostInfoDO> CostInfoDAO::selectAll(const CostInfoQuery::Wrapper& query) {

	SqlParams params;
	string charge = ZH_WORDS_GETTER("rhqm.form.cost_info.other.charge");
	string refund = ZH_WORDS_GETTER("rhqm.form.cost_info.other.refund");

	string sqlStr = "SELECT DISTINCT" MAIN_SQL_COLUMNS;
	// 收费退费为了中文表示需要拆开sql
	sqlStr += ", IF(aci.`refund_id` IS NULL, '" + charge + "', '" + refund + "') AS source";
	sqlStr += MAIN_SQL_JOINS;
	sqlStr += queryConditionBuilder(query, params);
	sqlStr += " ORDER BY aci.`occurrence_time` DESC, aci.`id` DESC ";

	return sqlSession->executeQuery<CostInfoDO>(sqlStr, CostInfoMapper(), params);
}

double CostInfoDAO::calcuSummary(const CostInfoQuery::Wrapper& query) {

	SqlParams params;
	// charge rows add, refund rows subtract; sum in cents to keep 2 decimals (executeQueryNumerical returns uint64_t)
	string sqlStr = "SELECT FLOOR(SUM(IF(t.is_refund, -t.receivableAmount, t.receivableAmount)) * 100 + 0.5) \
					FROM (SELECT DISTINCT (aci.`refund_id` IS NULL) AS is_refund,\
					 acid.`price_value` AS originalPrice,\
					 aci.`discount_amount` AS discountAmount, aci.`base_amount` AS receivableAmount,\
					 aci.`system_discount_price` AS discountedPrice, aci.`total_price` AS lineAmount,\
					 fpr.`tendered_amount` AS actualAmount, ysr.`fund_pay_sumamt` AS insuranceAmount,\
					 ysr.`psn_cash_pay` AS selfPayAmount ";
	sqlStr += MAIN_SQL_JOINS;
	sqlStr += queryConditionBuilder(query, params);
	sqlStr += " ) t";

	uint64_t cents = sqlSession->executeQueryNumerical(sqlStr, params);
	return static_cast<int64_t>(cents) / 100.0;
}
