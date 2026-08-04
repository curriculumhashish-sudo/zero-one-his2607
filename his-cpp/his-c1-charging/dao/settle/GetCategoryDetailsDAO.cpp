#include "stdafx.h"
#include "GetCategoryDetailsDAO.h"
#include "GetCategoryDetailsMapper.h"

// Build dynamic WHERE clause from query filters
inline std::string GetCategoryDetailsDAO::queryConditionBuilder(const GetCategoryDetailsQuery::Wrapper& query, SqlParams& params)
{
	std::ostringstream sqlCondition;
	sqlCondition << " WHERE ci.is_deleted = 0 ";

	// 就诊ID（核心筛选）
	if (query->encounter_id && !query->encounter_id->empty())
	{
		sqlCondition << " AND ci.encounter_id = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->encounter_id);
	}
	// 住院号筛选
	if (query->inpatient_no && !query->inpatient_no->empty())
	{
		sqlCondition << " AND e.inpatient_no = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, query->inpatient_no);
	}
	return sqlCondition.str();
}

// Count matching rows
uint64_t GetCategoryDetailsDAO::count(const GetCategoryDetailsQuery::Wrapper& query)
{
	SqlParams params;
	std::ostringstream sql;

	sql << " SELECT COUNT(DISTINCT cid.charge_type_name) "
		<< " FROM adm_charge_item ci "
		<< " LEFT JOIN adm_charge_item_definition cid ON ci.definition_id = cid.id "
		<< " LEFT JOIN adm_encounter e ON e.encounter_id = ci.encounter_id ";

	sql << queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql.str(), params);
}

// 费用大类分组汇总查询
std::list<GetCategoryDetailsDO> GetCategoryDetailsDAO::selectWithPage(const GetCategoryDetailsQuery::Wrapper& query)
{
	SqlParams params;
	std::ostringstream sql;

	sql << " SELECT "
		<< " ci.encounter_id,"
		<< " cid.sort_num,"
		<< " cid.charge_type_name, "
		<< " SUM(ci.total_price) AS type_total_fee "
		<< " FROM adm_charge_item ci "
		<< " LEFT JOIN adm_charge_item_definition cid ON ci.definition_id = cid.id "
		<< " LEFT JOIN adm_encounter e ON e.encounter_id = ci.encounter_id ";

	sql << queryConditionBuilder(query, params);

	sql << " GROUP BY ci.encounter_id, cid.sort_num, cid.charge_type_name "
		<< " ORDER BY cid.sort_num ";

	GetCategoryDetailsMapper mapper;
	return sqlSession->executeQuery<GetCategoryDetailsDO>(
		sql.str(), GetCategoryDetailsMapper(), params);
}
