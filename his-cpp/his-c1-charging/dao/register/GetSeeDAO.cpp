
#include"GetSeeDAO.h"
#include "GetSeeMapper.h"

inline std::string GetSeeDAO::queryConditionBuilder(const GetSeeQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->number) {

		sqlCondition << " AND `bus_no`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->number.getValue(""));
	}
	if (query->name) {

		sqlCondition << " AND name=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->name.getValue(""));
	}
	if (query->start_time) {

		sqlCondition << " AND time>=?";
		SQLPARAMS_PUSH(params, "dt", unsigned long long, query->start_time.getValue(1));
	}
	if (query->end_time) {

		sqlCondition << " AND time<=?";
		SQLPARAMS_PUSH(params, "dt",unsigned long long, query->start_time.getValue(253370736000));
	}
	return sqlCondition.str();
}


uint64_t GetSeeDAO::count(const GetSeeQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM adm_encounter e ";
	sql += "LEFT JOIN adm_patient p ON e.patient_id = p.id ";

	sql += "LEFT JOIN adm_organization o ON e.organization_id = o.id ";

	sql += "LEFT JOIN adm_practitioner pr ON e.registrar_id = pr.id ";

	sql += "LEFT JOIN adm_account acc ON e.id = acc.encounter_id ";

	sql += "LEFT JOIN fin_contract fc ON acc.contract_no = fc.bus_no ";

	sql += "LEFT JOIN adm_healthcare_service h ON acc.service_type_id  = h.id ";

	sql += "LEFT JOIN adm_charge_item ci ON e.id = ci.encounter_id AND ci.status_enum = 'ACTIVE' ";

	sql += "LEFT JOIN yb_encounter_platform_map map ON e.id = map.encounter_id ";

	sql += "LEFT JOIN yb_person_cache cache ON map.psn_no = cache.psn_no AND map.contract_id = cache.contract_id";

	sql += "LEFT JOIN yb_platform_dict_item dict ON dict.dict_value = cache.insuplc_admdvs AND dict.dict_type = \'INSUPLC_ADMDVS\'";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<GetSeeDO> GetSeeDAO::selectSeeWithPage(const GetSeeQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT e.id AS id, \
						e.bus_no AS bus_no,\
						p.name AS name,\
						p.gender_enum AS gender_enum,\
						p.phone AS phone,\
						o.name AS org_name,\
						pr.name AS doctor,\
						fc.name AS cost_type,\
						dict.dict_label AS addr\
						h.name AS register_type,\
						SUM(ci.total_price) AS cost,\
						e.start_time AS time,\
						FROM adm_encounter e ";

	sql += "LEFT JOIN adm_patient p ON e.patient_id = p.id ";

	sql += "LEFT JOIN adm_organization o ON e.organization_id = o.id ";

	sql += "LEFT JOIN adm_practitioner pr ON e.registrar_id = pr.id ";

	sql += "LEFT JOIN adm_account acc ON e.id = acc.encounter_id ";

	sql += "LEFT JOIN fin_contract fc ON acc.contract_no = fc.bus_no ";

	sql += "LEFT JOIN adm_healthcare_service h ON acc.service_type_id  = h.id ";

	sql += "LEFT JOIN adm_charge_item ci ON e.id = ci.encounter_id AND ci.status_enum = 'ACTIVE' ";

	sql +="LEFT JOIN yb_encounter_platform_map map ON e.id = map.encounter_id ";

	sql += "LEFT JOIN yb_person_cache cache ON map.psn_no = cache.psn_no AND map.contract_id = cache.contract_id";

	sql += "LEFT JOIN yb_platform_dict_item dict ON dict.dict_value = cache.insuplc_admdvs AND dict.dict_type = \'INSUPLC_ADMDVS\'";

	sql += queryConditionBuilder(query, params);


	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	return sqlSession->executeQuery<GetSeeDO>(sql, GetSeeMapper(), params);
}