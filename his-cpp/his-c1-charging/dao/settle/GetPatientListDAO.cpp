#include "stdafx.h"
#include "GetPatientListDAO.h"
#include "GetPatientListMapper.h"


inline std::string GetPatientListDAO::queryConditionBuilder(const GetPatientListQuery::Wrapper& query, SqlParams& params)
{
	std::ostringstream sqlCondition;
	sqlCondition << " WHERE 1=1 ";

	// 住院号精确匹配
	if (query->inpatient_no && !query->inpatient_no->empty())
	{
		sqlCondition << " AND e.inpatient_no = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->inpatient_no);
	}
	// 患者姓名模糊查询
	if (query->patient_name && !query->patient_name->empty())
	{
		sqlCondition << " AND p.patient_name LIKE ? ";
		SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->patient_name) + "%");
	}
	// 就诊状态筛选
	if (query->encounter_status)
	{
		sqlCondition << " AND e.encounter_status = ? ";
		SQLPARAMS_PUSH(params, "i", uint32_t, query->encounter_status);
	}

	return sqlCondition.str();
}


uint64_t GetPatientListDAO::count(const GetPatientListQuery::Wrapper& query)
{
	SqlParams params;
	std::ostringstream sql;

	sql << "SELECT COUNT(DISTINCT e.encounter_id) "
		<< " FROM adm_encounter e "
		<< " LEFT JOIN adm_patient p ON e.patient_id = p.id ";
	sql << queryConditionBuilder(query, params);

	return getSession()->executeQueryNumerical(sql.str(), params);
}


std::list<GetPatientListDO> GetPatientListDAO::selectWithPage(const GetPatientListQuery::Wrapper& query)
{
	SqlParams params;
	std::ostringstream sql;

	sql << "SELECT "
		<< " e.encounter_id,"
		<< " e.inpatient_no,"
		<< " p.patient_name,"
		<< " p.gender,"
		<< " p.age,"
		<< " e.dept_name,"
		<< " e.ward_name,"
		<< " e.bed_no,"
		<< " e.admit_date,"
		<< " e.insurance_type,"
		<< " e.encounter_status "
		<< " FROM adm_encounter e "
		<< " LEFT JOIN adm_patient p ON e.patient_id = p.id ";

	sql << queryConditionBuilder(query, params);

	sql << " ORDER BY e.admit_date DESC "
		<< " LIMIT " << std::to_string((int64_t)(query->pageIndex.getGetValue(1) - 1) * query->pageSize.getGetValue(10))
		<< " ," << std::to_string((int64_t)query->pageSize.getGetValue(10));

	GetPatientListMapper mapper;
	return getSession()->executeQuery<GetPatientListDO>(sql.str(), mapper, params);
}

