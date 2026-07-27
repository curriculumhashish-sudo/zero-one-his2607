// ======================================================
// GetCategoryDetailsDAO.h - 获取费用大类明细 - 数据访问对象
// 继承 BaseDAO，获得 sqlSession（数据库会话）、executeQuery 等方法
// 按 DAO 规范只提供查询方法
// 不使用父类 insert/update/delete，因为 DO 对应 JOIN 分组查询而非单表
// ======================================================
#pragma once
#include "BaseDAO.h"
#include "../../domain/do/settle/GetCategoryDetailsDO.h"
#include "../../domain/query/settle/GetCategoryDetailsQuery.h"

class GetCategoryDetailsDAO : public BaseDAO
{
private:
	// queryConditionBuilder – 动态构建 WHERE 子句(私有辅助方法)
	// 根据前端传入的 inpatient_no、encounter_id 动态拼接 SQL 条件
	// 使用 SqlParams 进行参数查询，防止 SQL 注入
	// 返回值：" WHERE 1=1 AND (...) AND (...) " 格式的 SQL 片段
	inline std::string queryConditionBuilder(const GetCategoryDetailsQuery::Wrapper& query, SqlParams& params);

public:
	// 查询符合条件记录总数
	uint64_t count(const GetCategoryDetailsQuery::Wrapper& query);

	// 查询费用大类分组汇总列表
	std::list<GetCategoryDetailDO> selectWithPage(const GetCategoryDetailsQuery::Wrapper& query);
};
