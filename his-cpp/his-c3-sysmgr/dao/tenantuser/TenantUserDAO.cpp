#include "stdafx.h"
#include "TenantUserDAO.h"
#include "TenantUserMapper.h"
#include <sstream>

namespace
{
	class StringValueMapper : public Mapper<std::string>
	{
	public:
		std::string mapper(ResultSet* resultSet) const override
		{
			return resultSet->getString(1);
		}
	};

	std::string buildInClause(size_t count)
	{
		std::ostringstream oss;
		oss << "(";
		for (size_t i = 0; i < count; ++i)
		{
			if (i > 0) oss << ",";
			oss << "?";
		}
		oss << ")";
		return oss.str();
	}
}

uint64_t TenantUserDAO::count(const TenantUserQuery::Wrapper& query)
{
	std::string sql = "SELECT COUNT(ut.id) FROM whale_user_tenants ut INNER JOIN whale_users u ON ut.user_id = u.id WHERE ut.is_deleted = 0 AND u.is_deleted = 0 AND ut.tenant_id = ? ";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, query->tenantId.getValue(""));
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PtrTenantUserQueryDO> TenantUserDAO::selectPage(const TenantUserQuery::Wrapper& query)
{
	std::string sql = "SELECT ut.id, ut.tenant_id, ut.user_id, u.user_name, u.name, u.phone_number, u.email, u.is_active, ut.created_at, ut.created_by "
		"FROM whale_user_tenants ut INNER JOIN whale_users u ON ut.user_id = u.id "
		"WHERE ut.is_deleted = 0 AND u.is_deleted = 0 AND ut.tenant_id = ? ";
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, query->tenantId.getValue(""));
	sql += "ORDER BY ut.created_at DESC LIMIT ?, ?";
	SQLPARAMS_PUSH(params, "ull", uint64_t, (query->pageIndex.getValue(1) - 1) * query->pageSize.getValue(10));
	SQLPARAMS_PUSH(params, "ull", uint64_t, query->pageSize.getValue(10));
	return sqlSession->executeQuery<PtrTenantUserQueryDO>(sql, TenantUserMapper(), params);
}

std::list<std::string> TenantUserDAO::selectExistingUserIds(const std::list<std::string>& userIds)
{
	std::list<std::string> results;
	if (userIds.empty())
		return results;

	std::string sql = "SELECT id FROM whale_users WHERE is_deleted = 0 AND id IN " + buildInClause(userIds.size());
	SqlParams params;
	for (const auto& userId : userIds)
		SQLPARAMS_PUSH(params, "s", std::string, userId);
	auto rows = sqlSession->executeQuery<std::string>(sql, StringValueMapper(), params);
	for (auto& row : rows)
		results.push_back(row);
	return results;
}

std::list<std::string> TenantUserDAO::selectBoundUserIds(const std::string& tenantId, const std::list<std::string>& userIds)
{
	std::list<std::string> results;
	if (userIds.empty())
		return results;

	std::string sql = "SELECT user_id FROM whale_user_tenants WHERE is_deleted = 0 AND tenant_id = ? AND user_id IN " + buildInClause(userIds.size());
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);
	for (const auto& userId : userIds)
		SQLPARAMS_PUSH(params, "s", std::string, userId);
	auto rows = sqlSession->executeQuery<std::string>(sql, StringValueMapper(), params);
	for (auto& row : rows)
		results.push_back(row);
	return results;
}

int TenantUserDAO::softDeleteByTenantAndUserIds(const std::string& tenantId, const std::list<std::string>& userIds, const std::string& deletedBy, const std::string& deletedAt)
{
	if (userIds.empty())
		return 0;

	std::string sql = "UPDATE whale_user_tenants SET is_deleted = 1, deleted_by = ?, deleted_at = ?, updated_by = ?, updated_at = ? "
		"WHERE is_deleted = 0 AND tenant_id = ? AND user_id IN " + buildInClause(userIds.size());
	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
	SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
	SQLPARAMS_PUSH(params, "s", std::string, deletedBy);
	SQLPARAMS_PUSH(params, "dt", std::string, deletedAt);
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);
	for (const auto& userId : userIds)
		SQLPARAMS_PUSH(params, "s", std::string, userId);
	return sqlSession->executeUpdate(sql, params);
}
