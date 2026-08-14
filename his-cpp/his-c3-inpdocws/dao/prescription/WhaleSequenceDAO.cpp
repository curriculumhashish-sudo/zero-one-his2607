#include "stdafx.h"
#include "WhaleSequenceDAO.h"

uint64_t WhaleSequenceDAO::nextValue(const std::string& seqKey)
{
	// Step 1: ensure the row exists. ON DUPLICATE KEY UPDATE idles so a fresh
	// row gets current_value = 0; an existing row is left untouched.
	// 检查 BaseDAO 的 sqlSession 是否为空
	//if (this->getSqlSession() == nullptr) {
	//	// 假设 BaseDAO 有 getSqlSession() 方法
	//	throw std::runtime_error("sqlSession is NULL in WhaleSequenceDAO");
	//}
	//else {
	//	cout << "sqlSession is not NULL" << endl;
	//}
	//std::cout << "sqlSession address: " << this->sqlSession << std::endl;
	//std::cout << "Testing if sqlSession is alive..." << std::endl;

	SqlParams upsertParams;
	std::string upsertSql =
		"INSERT INTO whale_sequence (seq_key, current_value, updated_at) "
		"VALUES (?, 0, NOW(6)) "
		"ON DUPLICATE KEY UPDATE seq_key = seq_key";
	SQLPARAMS_PUSH(upsertParams, "s", std::string, seqKey);
	sqlSession->executeUpdate(upsertSql, upsertParams);

	// Step 2: atomic increment-and-remember.
	// `LAST_INSERT_ID(expr)` writes `expr` back into current_value AND stashes
	// the new value in the connection's session-local LAST_INSERT_ID, so a
	// follow-up SELECT LAST_INSERT_ID() returns it without a race.
	SqlParams updateParams;
	std::string updateSql =
		"UPDATE whale_sequence "
		"SET current_value = LAST_INSERT_ID(current_value + 1), "
		"    updated_at    = NOW(6) "
		"WHERE seq_key = ?";
	SQLPARAMS_PUSH(updateParams, "s", std::string, seqKey);
	sqlSession->executeUpdate(updateSql, updateParams);

	// Step 3: read back the new value on the same connection.
	SqlParams selectParams;
	std::string selectSql = "SELECT LAST_INSERT_ID()";
	return sqlSession->executeQueryNumerical(selectSql, selectParams);
}
