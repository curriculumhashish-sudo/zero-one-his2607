#include "stdafx.h"
#include "PatientDAO.h"
#include "PatientMapper.h"

std::string PatientDAO::buildSelectCondition(const PatSelectPageQuery::Wrapper& query, SqlParams& params)
{
	std::string sql = " WHERE is_deleted = 0 AND active_flag = 1";

	if (query->keyword && !query->keyword->empty())
	{
		sql += " AND (name LIKE ? OR bus_no LIKE ? OR id_card LIKE ?)";
		std::string likeStr = "%" + query->keyword.getValue("") + "%";
		SQLPARAMS_PUSH(params, "s", std::string, likeStr);
		SQLPARAMS_PUSH(params, "s", std::string, likeStr);
		SQLPARAMS_PUSH(params, "s", std::string, likeStr);
	}
	return sql;
}

std::list<PtrPatientDO> PatientDAO::selectPatList(const PatSelectPageQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT bus_no, name, gender_enum, birth_date, phone, id_card "
		"FROM adm_patient";
	sql += buildSelectCondition(query, params);
	sql += " ORDER BY created_at DESC";

	uint64_t pageIndex = query->pageIndex.getValue(1);
	uint64_t pageSize = query->pageSize.getValue(20);
	if (pageIndex < 1) pageIndex = 1;
	if (pageSize < 1) pageSize = 20;

	uint64_t offset = (pageIndex - 1) * pageSize;
	sql += " LIMIT " + std::to_string(pageSize) + " OFFSET " + std::to_string(offset);

	return sqlSession->executeQuery<PtrPatientDO>(sql, PtrPatientSelectMapper(), params);
}

uint64_t PatientDAO::countPatList(const PatSelectPageQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM adm_patient";
	sql += buildSelectCondition(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::string PatientDAO::buildPatPageCondition(const PatPageQuery::Wrapper& query, SqlParams& params)
{
	std::string sql = " WHERE is_deleted = 0";

	// Name fuzzy match
	if (query->name && !query->name->empty())
	{
		sql += " AND name LIKE ?";
		std::string likeStr = "%" + query->name.getValue("") + "%";
		SQLPARAMS_PUSH(params, "s", std::string, likeStr);
	}

	// Id card fuzzy match
	if (query->idCard && !query->idCard->empty())
	{
		sql += " AND id_card LIKE ?";
		std::string likeStr = "%" + query->idCard.getValue("") + "%";
		SQLPARAMS_PUSH(params, "s", std::string, likeStr);
	}
	return sql;
}

std::list<PtrPatientDO> PatientDAO::queryPatPage(const PatPageQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT name, gender_enum, birth_date, phone, id_card "
		"FROM adm_patient";
	sql += buildPatPageCondition(query, params);
	sql += " ORDER BY created_at DESC";

	uint64_t pageIndex = query->pageIndex.getValue(1);
	uint64_t pageSize = query->pageSize.getValue(10);
	if (pageIndex < 1) pageIndex = 1;
	if (pageSize < 1) pageSize = 10;

	uint64_t offset = (pageIndex - 1) * pageSize;
	sql += " LIMIT " + std::to_string(pageSize) + " OFFSET " + std::to_string(offset);

	return sqlSession->executeQuery<PtrPatientDO>(sql, PtrPatientPageMapper(), params);
}

uint64_t PatientDAO::countPatPage(const PatPageQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql = "SELECT COUNT(*) FROM adm_patient";
	sql += buildPatPageCondition(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

// ============================================================
// 软删除患者 — 逻辑删除，不物理删除数据
// SQL说明:
//   - SET is_deleted=1 标记删除
//   - deleted_at/deleted_by 记录删除审计信息
//   - updated_at/updated_by 同时更新（历史兼容，取deleted_at/deleted_by相同值）
//   - WHERE is_deleted=0 防止重复删除
// 注意: 未使用乐观锁version校验，简化处理
// ============================================================
int PatientDAO::softDeletePatient(const std::string& id,
	const std::string& deletedAt,
	const std::string& deletedBy)
{
	const std::string sql = R"(
		UPDATE adm_patient
		SET is_deleted = 1,
			deleted_at = ?,
			deleted_by = ?,
			updated_at = ?,
			updated_by = ?
		WHERE id = ?
		  AND is_deleted = 0
	)";

	SqlParams params;
	SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
	SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
	SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
	SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
	SQLPARAMS_PUSH(params, "s", std::string, id);

	return sqlSession->executeUpdate(sql, params);
}
