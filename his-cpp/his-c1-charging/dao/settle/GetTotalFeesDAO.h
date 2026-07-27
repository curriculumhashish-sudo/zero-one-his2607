// ======================================================
// GetTotalFeesDAO.h - 获取患者费用合计 - 数据访问对象
// 继承 BaseDAO，获得 sqlSession（数据库会话）、executeQuery 等方法
// 按 DAO 规范只提供查询方法
// 不使用父类 insert/update/delete，因为 DO 对应 JOIN 汇总查询而非单表
// ======================================================
#pragma once
#include "BaseDAO.h"
#include "../../domain/do/settle/GetTotalFeesDO.h"
#include "../../domain/query/settle/GetTotalFeesQuery.h"

class GetTotalFeesDAO : public BaseDAO
{
private:
	// queryConditionBuilder – 动态构建 WHERE 子句(私有辅助方法)
	// 根据前端传入的 inpatient_no 动态拼接 SQL 条件
	// 使用 SqlParams 进行参数查询，防止 SQL 注入
	// 返回值：" WHERE 1=1 AND (...) " 格式的 SQL 片段
	inline std::string queryConditionBuilder(const GetTotalFeesQuery::Wrapper& query, SqlParams& params);

public:
	// 查询符合条件记录总数
	uint64_t count(const GetTotalFeesQuery::Wrapper& query);

	// 查询费用合计汇总数据
	std::list<GetTotalFeesDO> selectWithPage(const GetTotalFeesQuery::Wrapper& query);
};
