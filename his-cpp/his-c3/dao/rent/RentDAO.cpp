#include "stdafx.h"
#include "RentDAO.h"
#include "RentMapper.h"

std::string RentDAO::queryConditionBuilder(const RentQuery::Wrapper& query, SqlParams& params) {
	stringstream sqlCondition;
	sqlCondition << " WHERE is_deleted=0";
	if (query->code) {

		sqlCondition << " AND code like ?";
		SQLPARAMS_PUSH(params, "s", std::string, "%"+ query->code.getValue("")+"%");
	}
	if (query->keywords) {

		sqlCondition << " AND (name like ? OR description like ?)";
		SQLPARAMS_PUSH(params, "s", std::string,"%"+ query->keywords.getValue("")+"%");
		SQLPARAMS_PUSH(params, "s", std::string,"%"+ query->keywords.getValue("")+"%");
	}
	if (query->status) {

		sqlCondition << " AND is_active= ? ";
		SQLPARAMS_PUSH(params, "i", int, query->status.getValue(0));
	}
	return sqlCondition.str();
}

uint64_t RentDAO::countName()
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM whale_tenants WHERE is_deleted=0";
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

uint64_t RentDAO::count(const RentQuery::Wrapper& query) {
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM whale_tenants ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}


std::list<RentDO> RentDAO::selectWithPage(const RentQuery::Wrapper& query) {
	SqlParams params;
	string sql = "SELECT id,name,code,description,is_active,extra_properties,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by FROM whale_tenants ";
	// 构建查询条件
	sql += queryConditionBuilder(query, params);
	// 构建排序语句
	sql += " ORDER BY IFNULL(`updated_at`, `created_at`) DESC, `id` ASC ";

	// 构建分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	cout << sql << endl;
	// 执行查询
	return sqlSession->executeQuery<RentDO>(sql, RentMapper(), params);
}

std::list<RentDO> RentDAO::selectAllName(){
	SqlParams params;
	string sql = "SELECT id,name,code FROM whale_tenants WHERE is_deleted=0";
	// 构建排序语句
	sql += " ORDER BY `id` ASC ";

	// 执行查询
	return sqlSession->executeQuery<RentDO>(sql, RentNameMapper(), params);
}
