#ifndef _NURSE_DAO2_
#define _NURSE_DAO2_
#include "BaseDAO.h"
#include "domain/do/nurse2/nurseDO2.h"
#include "domain/query/nurse2/NurseQuery2.h"

/**
 * Nurse2 数据访问对象
 * - 仅封装各类查询方法 —— 新增、修改、删除逻辑全部继承自基础父类 BaseDAO
 */
class NurseDAO2 : public BaseDAO
{
private:
	// 根据查询筛选条件拼接生成 WHERE 子句
	inline std::string queryConditionBuilder(const NurseQuery2::Wrapper& query, SqlParams& params);

public:
	// 根据查询过滤条件统计数据条数
	uint64_t count(const NurseQuery2::Wrapper& query);
	//  分页查询数据
	std::list<NurseDO2> selectWithPage(const NurseQuery2::Wrapper& query);
};

#endif // !_NURSE_DAO2_
