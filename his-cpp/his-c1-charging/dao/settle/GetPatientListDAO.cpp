#include "stdafx.h"
#include "GetPatientListDAO.h"
#include "GetPatientListMapper.h"


inline std::string GetPatientListDAO::queryConditionBuilder(const GetPatientListQuery::Wrapper& query, SqlParams& params)
{
	std::ostringstream sqlCondition;
	sqlCondition << " WHERE 1=1 ";

	// 就诊ID
	if (query->encounter_id)
	{
		sqlCondition << " AND e.encounter_id = ? ";
		SQLPARAMS_PUSH(params, "ll", uint64_t, query->encounter_id);
	}
	// 住院号 精确匹配
	if (query->inpatient_no && !query->inpatient_no->empty())
	{
		sqlCondition << " AND e.inpatient_no = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->inpatient_no);
	}
	// 患者姓名 模糊查询
	if (query->patient_name && !query->patient_name->empty())
	{
		sqlCondition << " AND p.patient_name LIKE ? ";
		SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->patient_name) + "%");
	}
	// 性别
	if (query->gender)
	{
		sqlCondition << " AND p.gender = ? ";
		SQLPARAMS_PUSH(params, "i", uint8_t, query->gender);
	}
	// 年龄
	if (query->age)
	{
		sqlCondition << " AND p.age = ? ";
		SQLPARAMS_PUSH(params, "i", uint32_t, query->age);
	}
	// 科室名称
	if (query->dept_name && !query->dept_name->empty())
	{
		sqlCondition << " AND e.dept_name LIKE ? ";
		SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->dept_name) + "%");
	}
	// 病区名称
	if (query->ward_name && !query->ward_name->empty())
	{
		sqlCondition << " AND e.ward_name LIKE ? ";
		SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->ward_name) + "%");
	}
	// 床号
	if (query->bed_no && !query->bed_no->empty())
	{
		sqlCondition << " AND e.bed_no = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->bed_no);
	}
	// 入院时间
	if (query->admit_date)
	{
		sqlCondition << " AND e.admit_date = ? ";
		SQLPARAMS_PUSH(params, "ll", uint64_t, query->admit_date);
	}
	// 医保类型
	if (query->insurance_type)
	{
		sqlCondition << " AND e.insurance_type = ? ";
		SQLPARAMS_PUSH(params, "i", uint8_t, query->insurance_type);
	}
	// 医生姓名
	if (query->doctor_name && !query->doctor_name->empty())
	{
		sqlCondition << " AND e.doctor_name LIKE ? ";
		SQLPARAMS_PUSH(params, "s", std::string, "%" + (*query->doctor_name) + "%");
	}

	return sqlCondition.str();
}

// Count matching rows
uint64_t GetPatientListDAO::count(const GetPatientListQuery::Wrapper& query)
{
	SqlParams params;
	std::ostringstream sql;

	sql << " SELECT COUNT(DISTINCT e.encounter_id) "
		<< " FROM adm_encounter e "
		<< " LEFT JOIN adm_patient p ON e.patient_id = p.id ";
	sql << queryConditionBuilder(query, params);

	return sqlSession->executeQueryNumerical(sql.str(), params);
}

// Paginated query – multi-table JOIN
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
		<< " e.doctor_name "
		<< " FROM adm_encounter e "
		<< " LEFT JOIN adm_patient p ON e.patient_id = p.id ";

	sql << queryConditionBuilder(query, params);

	sql << " ORDER BY e.admit_date DESC "
		<< " LIMIT " << std::to_string((int64_t)(query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10))
		<< " ," << std::to_string((int64_t)query->pageSize.getValue(10));

	GetPatientListMapper mapper;
	return sqlSession->executeQuery<GetPatientListDO>(
		sql.str(), GetPatientListMapper(), params);
}


