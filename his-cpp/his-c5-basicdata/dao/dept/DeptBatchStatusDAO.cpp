#include "DeptBatchStatusDao.h"
#include "stdafx.h"
#include "SqlSession.h"

int DeptBatchStatusDAO::batchUpdateStatus(const DeptBatchUpdateDTO::Wrapper& updateDto)
{
	if (!updateDto->idList || updateDto->idList->empty())
	{
		return 0;
	}

	// 初始化数据库会话
	SqlSession* session = new SqlSession();
	setSqlSession(session);
	setAutoRelease(true);

	SqlParams params;
	// MySQL批量更新SQL
	std::string sql = R"(
		UPDATE adm\_organization
		SET 
			status\_enum = ?,
			active\_flag = ?,
			updated\_by = ?,
			updated\_at = ?,
			version = version + 1
		WHERE id IN (?)
	)";

	try
	{
		session->beginTransaction();

		// 依次填充固定字段参数
		SQLPARAMS_PUSH(params, "s", std::string, updateDto->statusEnum->c_str());
		SQLPARAMS_PUSH(params, "b", bool, updateDto->activeFlag);
		SQLPARAMS_PUSH(params, "s", std::string, updateDto->updateBy->c_str());
		SQLPARAMS_PUSH(params, "dt", std::string, updateDto->updateTime->c_str());

		// 拼接ID字符串 放入IN条件
		std::string idStr;
		bool first = true;
		for (auto id : * (updateDto->idList))
		{
			if (!first) idStr += ",";
			first = false;
			idStr += std::to_string(id);
		}
		SQLPARAMS_PUSH(params, "s", std::string, idStr);

		// 执行更新
		int rows = session->executeUpdate(sql, params);

		if (rows == updateDto->idList->size())
		{
			session->commitTransaction();
			return rows;
		}
		else
		{
			session->rollbackTransaction();
			return 0;
		}
	}
	catch (const std::exception& e)
	{
		session->rollbackTransaction();
		OATPP_LOGE("DeptDao", "批量更新科室异常:%s", e.what());
		return 0;
	}
}