#include "stdafx.h"
#include "RefundGetItemDAO.h"
#include "RefundGetItemMapper.h"

inline std::string RefundGetItemDAO::queryConditionBuilder(const RefundGetItemQuery::Wrapper& query, SqlParams& params) {
	return std::string();
}

// 分页查询退费项目明细列表（扁平行）
// 参数: 前端传入的查询条件（encounterId + detailViewCode）
// 返回: 扁平 DO 列表，由 Service 层分组为嵌套结构
std::list<RefundGetItemDO> RefundGetItemDAO::selectWithPage(const RefundGetItemQuery::Wrapper& query) {
	return std::list<RefundGetItemDO>();
}

// 统计满足条件的记录总数
uint64_t RefundGetItemDAO::count(const RefundGetItemQuery::Wrapper& query) {
	return 0;
}

