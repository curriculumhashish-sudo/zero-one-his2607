#include "stdafx.h"
#include "EquipmentDAO.h"
#include "EquipmentMapper.h"

std::string EquipmentDAO::queryConditionBuilder(const EquipmentQuery::Wrapper& query, SqlParams& params)
{
	return {};
}

uint64_t EquipmentDAO::count(const EquipmentQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM sample ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

//std::list<SampleDO> SampleDAO::selectWithPage(const SampleQuery::Wrapper& query)
//{
//	SqlParams params;
//	string sql = "SELECT id,name,sex,age FROM sample ";
//	// 构建查询条件
//	sql += queryConditionBuilder(query, params);
//	// 构建排序语句
//	sql += " ORDER BY IFNULL(`update_time`, `create_time`) DESC, `id` DESC ";
//	// 构建分页条件
//	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);
//
//	// 执行查询
//	return sqlSession->executeQuery<SampleDO>(sql, SampleMapper(), params);
//}
//
//std::list<SampleDO> SampleDAO::selectByName(const string& name)
//{
//	string sql = "SELECT id,name,sex,age FROM sample WHERE `name` LIKE CONCAT('%',?,'%')";
//	return sqlSession->executeQuery<SampleDO>(sql, SampleMapper(), "%s", name);
//}
//
//PtrSampleDO SampleDAO::selectById(std::string id)
//{
//	string sql = "SELECT id,name,sex,age FROM sample WHERE `id`=?";
//	return sqlSession->executeQueryOne<PtrSampleDO>(sql, PtrSampleMapper(), "%s", id);
//}
