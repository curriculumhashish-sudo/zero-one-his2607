#pragma once
#include "BaseDAO.h"
#include "../../domain/do/settle/GetPatientListDO.h"
#include "../../domain/query/settle/GetPatientListQuery.h"

/**
 * 患者列表DAO
 * 多表分页查询，不使用BaseDO自带单表CRUD
 */
class GetPatientListDAO : public BaseDAO
{
private:

	inline std::string queryConditionBuilder(const GetPatientListQuery::Wrapper& query, SqlParams& params);

public:
	// 查询符合条件的记录总数（用于前端分页组件显示总条数）
	uint64_t count(const GetPatientListQuery::Wrapper& query);

	// 分页查询记录列表（含排序 + LIMIT 分页）
	std::list<GetPatientListDO> selectWithPage(const GetPatientListQuery::Wrapper& query);
};
