#include "PageDiagnosisDictionaryDAO.h"
#include "stdafx.h"

std::list<PtrClnConditionDefinitionDO> PageDiagnosisDictionaryDAO::queryDiagnosisDictionary(
    const PageDiagnosisDictionaryQuery::Wrapper& query,
    const std::string& tenantId,
    const std::string& practitionerId)
{
	std::string sql = R"(
		SELECT 
			id,
			tenant_id,
			condition_code,
			name,
			type_code
		FROM cln_condition_definition
		WHERE is_deleted = 0
	)";

	SqlParams params;
	if (!tenantId.empty())
	{
		sql += " AND tenant_id = ? ";
		SQLPARAMS_PUSH(params, "s", std::string, tenantId);
	}

	// keyword：只匹配诊断名称、诊断编码，去掉py_str、wb_str
	if (query->keyword && !query->keyword->empty())
	{
		sql += R"(
		AND (
			name LIKE CONCAT('%', ?, '%')
			OR condition_code LIKE CONCAT('%', ?, '%')
			)
		)";

		// 把关键字先拷贝到局部 std::string，再传入宏，避免传入临时表达式导致参数失效
		std::string kw = query->keyword->c_str();
		SQLPARAMS_PUSH(params, "s", std::string, kw);
		SQLPARAMS_PUSH(params, "s", std::string, kw);
	}

	PageDiagnosisDictionaryMapper mapper;
	return sqlSession->executeQuery<PtrClnConditionDefinitionDO>(sql, mapper, params);
}
