/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/21
*/
#include "stdafx.h"
#include "PrintLogDAO.h"
#include "PrintLogMapper.h"

std::string PrintLogDAO::queryConditionBuilder(const PrintLogQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->docType) {
		sqlCondition << " AND doc_type=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->docType.getValue(""));
	}
	if (query->pname) {
		sqlCondition << " AND patient_name LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->pname.getValue(""));
	}
	if (query->isReprint) {
		sqlCondition << " AND is_reprint=?";
		SQLPARAMS_PUSH(params, "i", int, query->isReprint.getValue(0));
	}
	if (query->timefrom) {
		sqlCondition << " AND print_time>=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->timefrom.getValue(""));
	}
	if (query->timeto) {
		sqlCondition << " AND print_time<=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->timeto.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t PrintLogDAO::count(const PrintLogQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM print_log ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PrintLogDO> PrintLogDAO::selectWithPage(const PrintLogQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT id,print_job_id,doc_type,patient_name,print_time,"
		"printer_name,is_reprint,status,operator,remark,create_time FROM print_log ";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY print_time DESC, id DESC ";
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize))
		+ "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<PrintLogDO>(sql, PrintLogMapper(), params);
}
