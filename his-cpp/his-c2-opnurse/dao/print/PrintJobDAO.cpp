/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#include "stdafx.h"
#include "PrintJobDAO.h"
#include "PrintJobMapper.h"

std::string PrintJobDAO::queryConditionBuilder(const PrintJobQuery::Wrapper& query, SqlParams& params)
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
	if (query->dept) {
		sqlCondition << " AND exec_dept=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->dept.getValue(""));
	}
	if (query->doctor) {
		sqlCondition << " AND doctor_name LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->doctor.getValue(""));
	}
	if (query->st) {
		sqlCondition << " AND status=?";
		SQLPARAMS_PUSH(params, "i", int, query->st.getValue(0));
	}
	if (query->tmfrom) {
		sqlCondition << " AND order_time>=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->tmfrom.getValue(""));
	}
	if (query->tmto) {
		sqlCondition << " AND order_time<=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->tmto.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t PrintJobDAO::count(const PrintJobQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM print_job ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PrintJobDO> PrintJobDAO::selectWithPage(const PrintJobQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT id,doc_type,patient_name,treatment_item,exec_dept,doctor_name,"
		"order_time,exec_time,executor,status,print_count,remark,"
		"create_by,create_time,update_by,update_time FROM print_job ";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY IFNULL(`update_time`, `create_time`) DESC, `id` DESC ";
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize))
		+ "," + std::to_string(query->pageSize);
	return sqlSession->executeQuery<PrintJobDO>(sql, PrintJobMapper(), params);
}

PtrPrintJobDO PrintJobDAO::selectById(std::string id)
{
	string sql = "SELECT id,doc_type,patient_name,treatment_item,exec_dept,doctor_name,"
		"order_time,exec_time,executor,status,print_count,remark,"
		"create_by,create_time,update_by,update_time FROM print_job WHERE id=?";
	return sqlSession->executeQueryOne<PtrPrintJobDO>(sql, PtrPrintJobMapper(), "%s", id);
}
