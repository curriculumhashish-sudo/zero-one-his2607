#include "stdafx.h"
#include "SampleDAO.h"

static const char* SAMPLE_TABLE_NAME = "adm_specimen_definition";

class SampleMapper : public Mapper<PtrSampleDO>
{
public:
	PtrSampleDO mapper(ResultSet* resultSet) const override
	{
		auto sample = std::make_shared<SampleDO>();
		sample->setId(resultSet->getString("id"));
		sample->setTenantId(resultSet->getString("tenant_id"));
		sample->setCreateTime(resultSet->getString("created_at"));
		sample->setUpdateTime(resultSet->getString("updated_at"));
		sample->setCreateBy(resultSet->getString("created_by"));
		sample->setUpdateBy(resultSet->getString("updated_by"));
		sample->setIsDeleted(resultSet->getInt("is_deleted"));
		sample->setDeletedAt(resultSet->getString("deleted_at"));
		sample->setDeletedBy(resultSet->getString("deleted_by"));
		sample->setVersion(resultSet->getInt("version"));
		sample->setSampleCode(resultSet->getString("bus_no"));
		sample->setSampleName(resultSet->getString("name"));
		sample->setInputCode(resultSet->getString("py_str"));
		sample->setWbCode(resultSet->getString("wb_str"));
		sample->setTypeEnum(resultSet->getString("type_enum"));
		sample->setStatusEnum(resultSet->getString("status_enum"));
		sample->setRemark("");
		return sample;
	}
};

static std::string toStatusEnum(int status)
{
	return status == 1 ? "ACTIVE" : "INACTIVE";
}

static void appendKeywordWhere(std::ostringstream& sql, const std::string& keyword)
{
	sql << " WHERE is_deleted = 0";
	if (!keyword.empty())
	{
		sql << " AND (bus_no LIKE ?"
			<< " OR name LIKE ?"
			<< " OR py_str LIKE ?"
			<< " OR wb_str LIKE ?)";
	}
}

static void appendKeywordParams(SqlParams& params, const std::string& keyword)
{
	if (!keyword.empty())
	{
		const std::string likeKeyword = "%" + keyword + "%";
		SQLPARAMS_PUSH(params, "s", std::string, likeKeyword);
		SQLPARAMS_PUSH(params, "s", std::string, likeKeyword);
		SQLPARAMS_PUSH(params, "s", std::string, likeKeyword);
		SQLPARAMS_PUSH(params, "s", std::string, likeKeyword);
	}
}

static void pushNullableString(SqlParams& params, const std::shared_ptr<std::string>& value)
{
	if (value)
	{
		params.emplace_back(SqlParam("s", std::shared_ptr<void>(value)));
	}
	else
	{
		params.emplace_back(SqlParam("nul", std::make_shared<int>(1)));
	}
}

static void pushNullableDateTime(SqlParams& params, const std::shared_ptr<std::string>& value)
{
	if (value)
	{
		params.emplace_back(SqlParam("dt", std::shared_ptr<void>(value)));
	}
	else
	{
		params.emplace_back(SqlParam("nul", std::make_shared<int>(1)));
	}
}

std::list<PtrSampleDO> SampleDAO::selectByKeyword(const std::string& keyword, uint64_t offset, uint64_t limit)
{
	std::ostringstream sql;
	sql << "SELECT id, tenant_id, created_at, updated_at, created_by, updated_by, "
		<< "is_deleted, deleted_at, deleted_by, version, bus_no, name, py_str, wb_str, "
		<< "type_enum, status_enum "
		<< "FROM " << SAMPLE_TABLE_NAME;
	appendKeywordWhere(sql, keyword);
	sql << " ORDER BY updated_at DESC, created_at DESC, id DESC LIMIT ?, ?";

	SqlParams params;
	appendKeywordParams(params, keyword);
	SQLPARAMS_PUSH(params, "ull", uint64_t, offset);
	SQLPARAMS_PUSH(params, "ull", uint64_t, limit);

	return sqlSession->executeQuery<PtrSampleDO>(sql.str(), SampleMapper(), params);
}

uint64_t SampleDAO::countByKeyword(const std::string& keyword)
{
	std::ostringstream sql;
	sql << "SELECT COUNT(1) FROM " << SAMPLE_TABLE_NAME;
	appendKeywordWhere(sql, keyword);

	SqlParams params;
	appendKeywordParams(params, keyword);
	return sqlSession->executeQueryNumerical(sql.str(), params);
}

PtrSampleDO SampleDAO::selectById(const std::string& id)
{
	const std::string sql =
		"SELECT id, tenant_id, created_at, updated_at, created_by, updated_by, "
		"is_deleted, deleted_at, deleted_by, version, bus_no, name, py_str, wb_str, "
		"type_enum, status_enum "
		"FROM adm_specimen_definition WHERE id = ? AND is_deleted = 0";
	return sqlSession->executeQueryOne<PtrSampleDO>(sql, SampleMapper(), "%s", id);
}

bool SampleDAO::existsByCode(const std::string& sampleCode, const std::string& excludeId)
{
	if (excludeId.empty())
	{
		return sqlSession->executeQueryNumerical(
			"SELECT COUNT(1) FROM adm_specimen_definition WHERE bus_no = ? AND is_deleted = 0",
			"%s",
			sampleCode) > 0;
	}

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", std::string, sampleCode);
	SQLPARAMS_PUSH(params, "s", std::string, excludeId);
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(1) FROM adm_specimen_definition WHERE bus_no = ? AND id <> ? AND is_deleted = 0",
		params) > 0;
}

int SampleDAO::insertSample(const SampleDO& sample)
{
	SqlParams params;
	const std::string sql =
		"INSERT INTO adm_specimen_definition ("
		"id, tenant_id, created_at, updated_at, created_by, updated_by, "
		"is_deleted, deleted_at, deleted_by, version, "
		"bus_no, name, py_str, wb_str, type_enum, status_enum"
		") VALUES ("
		"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"
		")";

	SQLPARAMS_PUSH(params, "s", std::string, sample.getId());
	pushNullableString(params, sample.getTenantIdPtr());
	SQLPARAMS_PUSH(params, "dt", std::string, sample.getCreateTime());
	pushNullableDateTime(params, sample.getUpdatedAtPtr());
	pushNullableString(params, sample.getCreatedByPtr());
	pushNullableString(params, sample.getUpdatedByPtr());
	SQLPARAMS_PUSH(params, "i", int32_t, sample.getIsDeletedPtr() ? sample.getIsDeleted() : 0);
	pushNullableDateTime(params, sample.getDeletedAtPtr());
	pushNullableString(params, sample.getDeletedByPtr());
	SQLPARAMS_PUSH(params, "i", int32_t, sample.getVersionPtr() ? sample.getVersion() : 0);
	SQLPARAMS_PUSH(params, "s", std::string, sample.getSampleCode());
	SQLPARAMS_PUSH(params, "s", std::string, sample.getSampleName());
	pushNullableString(params, sample.getPyStrPtr());
	pushNullableString(params, sample.getWbStrPtr());
	SQLPARAMS_PUSH(params, "s", std::string, sample.getTypeEnum());
	SQLPARAMS_PUSH(params, "s", std::string, sample.getStatusEnum());

	return sqlSession->executeUpdate(sql, params);
}

int SampleDAO::updateSample(const SampleDO& sample)
{
	SqlParams params;
	std::ostringstream sql;
	sql << "UPDATE adm_specimen_definition SET "
		<< "bus_no = ?, name = ?";

	SQLPARAMS_PUSH(params, "s", std::string, sample.getSampleCode());
	SQLPARAMS_PUSH(params, "s", std::string, sample.getSampleName());

	if (sample.getPyStrPtr())
	{
		sql << ", py_str = ?";
		SQLPARAMS_PUSH(params, "s", std::string, sample.getInputCode());
	}

	if (sample.getWbStrPtr())
	{
		sql << ", wb_str = ?";
		SQLPARAMS_PUSH(params, "s", std::string, sample.getWbCode());
	}

	if (sample.getStatusEnumPtr())
	{
		sql << ", status_enum = ?";
		SQLPARAMS_PUSH(params, "s", std::string, sample.getStatusEnum());
	}

	sql << ", updated_by = ?, updated_at = ? "
		<< "WHERE id = ? AND is_deleted = 0";

	pushNullableString(params, sample.getUpdatedByPtr());
	SQLPARAMS_PUSH(params, "dt", std::string, sample.getUpdateTime());
	SQLPARAMS_PUSH(params, "s", std::string, sample.getId());

	return sqlSession->executeUpdate(sql.str(), params);
}

int SampleDAO::removeSampleById(const std::string& id)
{
	return sqlSession->executeUpdate(
		"UPDATE adm_specimen_definition "
		"SET is_deleted = 1, deleted_at = NOW(6), updated_at = NOW(6) "
		"WHERE id = ? AND is_deleted = 0",
		"%s",
		id);
}

int SampleDAO::updateStatusByIds(const std::list<std::string>& ids, int status, const std::string& updateBy, const std::string& updateTime)
{
	if (ids.empty())
	{
		return 0;
	}

	SqlParams params;
	std::ostringstream sql;
	sql << "UPDATE " << SAMPLE_TABLE_NAME << " SET status_enum = ?, updated_by = ?, updated_at = ? "
		<< "WHERE is_deleted = 0 AND id IN (";
	SQLPARAMS_PUSH(params, "s", std::string, toStatusEnum(status));
	SQLPARAMS_PUSH(params, "s", std::string, updateBy);
	SQLPARAMS_PUSH(params, "dt", std::string, updateTime);

	bool isFirst = true;
	for (const auto& id : ids)
	{
		if (!isFirst)
		{
			sql << ", ";
		}
		else
		{
			isFirst = false;
		}

		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, id);
	}
	sql << ")";
	return sqlSession->executeUpdate(sql.str(), params);
}
