#include "stdafx.h"
#include "PharmDAO.h"
#include "PharmMapper.h"
#include "PharmNameMapper.h"
#include <sstream>

std::string PharmDAO::queryConditionBuilder(const PharmQuery::Wrapper& query, SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE is_deleted = 0";

	if (query->keyword)
	{
		sqlCondition << " AND name LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->keyword.getValue(""));
	}

	if (query->type)
	{
		sqlCondition << " AND type = ?";
		SQLPARAMS_PUSH(params, "s", std::string, query->type.getValue(""));
	}

	if (query->status.getValue(-1) != -1)
	{
		sqlCondition << " AND status = ?";
		SQLPARAMS_PUSH(params, "i", int, query->status.getValue(0));
	}

	return sqlCondition.str();
}

uint64_t PharmDAO::count(const PharmQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM sys_pharm";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PtrPharmDO> PharmDAO::selectPage(const PharmQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql;
	sql = "SELECT id, name, type, status, sort, remark, create_by, update_by, create_time, update_time "
		"FROM sys_pharm";
	sql += queryConditionBuilder(query, params);
	sql += " ORDER BY sort ASC, id DESC";

	uint64_t offset = (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10);
	uint64_t size = query->pageSize.getValue(10);
	sql += " LIMIT " + std::to_string(offset) + ", " + std::to_string(size);

	return sqlSession->executeQuery<PtrPharmDO>(sql, PharmMapper(), params);
}

int PharmDAO::insert(const PharmDO& data)
{
	std::string sql =
		"INSERT INTO sys_pharm "
		"(name, type, status, sort, remark, create_by, update_by, create_time, update_time) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, data.getName());
	SQLPARAMS_PUSH(params, "s", std::string, data.getType());
	SQLPARAMS_PUSH(params, "i", int, data.getStatus());
	SQLPARAMS_PUSH(params, "i", int, data.getSort());
	SQLPARAMS_PUSH(params, "s", std::string, data.getRemark());
	SQLPARAMS_PUSH(params, "s", std::string, data.getCreate_By());
	SQLPARAMS_PUSH(params, "s", std::string, data.getUpdate_By());
	SQLPARAMS_PUSH(params, "s", std::string, data.getCreate_Time());
	SQLPARAMS_PUSH(params, "s", std::string, data.getUpdate_Time());

	return sqlSession->executeUpdate(sql, params);
}

int PharmDAO::update(const PharmDO& data)
{
	std::string sql =
		"UPDATE sys_pharm SET "
		"name = ?, type = ?, sort = ?, remark = ?, update_by = ?, update_time = ? "
		"WHERE id = ?;";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, data.getName());
	SQLPARAMS_PUSH(params, "s", std::string, data.getType());
	SQLPARAMS_PUSH(params, "i", int, data.getSort());
	SQLPARAMS_PUSH(params, "s", std::string, data.getRemark());
	SQLPARAMS_PUSH(params, "s", std::string, data.getUpdate_By());
	SQLPARAMS_PUSH(params, "s", std::string, data.getUpdate_Time());
	SQLPARAMS_PUSH(params, "ull", uint64_t, data.getId());

	return sqlSession->executeUpdate(sql, params);
}

int PharmDAO::remove(uint64_t id)
{
	std::string sql = "UPDATE sys_pharm SET is_deleted = 1 WHERE id = ?;";
	SqlParams params;
	SQLPARAMS_PUSH(params, "ull", uint64_t, id);
	return sqlSession->executeUpdate(sql, params);
}

std::list<PtrPharmDO> PharmDAO::nameList(const std::string& type)
{
    std::string sql = "SELECT id, name FROM sys_pharm WHERE is_deleted = 0 AND status = 1";
    SqlParams params;
    if (!type.empty())
    {
        sql += " AND type = ?";
        SQLPARAMS_PUSH(params, "s", std::string, type);
    }
    sql += " ORDER BY sort ASC, id DESC";
    return sqlSession->executeQuery<PtrPharmDO>(sql, PharmNameMapper(), params);
}

int PharmDAO::updateStatus(uint64_t id, int status)
{
	std::string sql = "UPDATE sys_pharm SET status = ? WHERE id = ?;";
	SqlParams params;
	SQLPARAMS_PUSH(params, "i", int, status);
	SQLPARAMS_PUSH(params, "ull", uint64_t, id);
	return sqlSession->executeUpdate(sql, params);
}