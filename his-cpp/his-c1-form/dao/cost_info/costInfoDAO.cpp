#include "stdafx.h"
#include "costInfoDAO.h"
#include "costInfoMapper.h"

std::string CostInfoDAO::queryConditionBuilder(const CostInfoQuery::Wrapper& query, SqlParams& params) {
	
	return "";
}

uint64_t CostInfoDAO::count(const CostInfoQuery::Wrapper& query) {

	return 0;
}
// 分页查询数据
std::list<CostInfoDO> CostInfoDAO::selectWithPage(const CostInfoQuery::Wrapper& query) {

	return NULL;
}
// 导出全部数据（不分页）
std::list<CostInfoDO> CostInfoDAO::selectAll(const CostInfoQuery::Wrapper& query) {

	return NULL;
}