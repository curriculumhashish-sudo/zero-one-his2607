#pragma once
#ifndef _DEPT_BATCH_STATUS_DTO_
#define _DEPT_BATCH_STATUS_DTO_
#include "../../GlobalInclude.h"
#include <cctype>
#include <unordered_set>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 前端入参DTO：批量启停科室请求对象
 * deptIdList / targetStatus 均标记为必填（Swagger required=true）
 * deptIdList 元素必须是字符串（雪花ID超出 JS Number 安全范围）
 * targetStatus：0=停用，1=启用
 */
class DeptBatchStatusDTO : public oatpp::DTO
{
	DTO_INIT(DeptBatchStatusDTO, DTO);

	// 批量科室ID集合（必填）
	API_DTO_FIELD_REQUIRE(Vector<String>, deptIdList, ZH_WORDS_GETTER("dept.field.deptIdList"), true)

	// 目标启停状态（必填）
	API_DTO_FIELD_REQUIRE(Int32, targetStatus, ZH_WORDS_GETTER("dept.field.targetStatus"), true)

	// 当前登录操作人载荷
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
	/**
	 * 业务层入参校验（在 JSON 反序列化成功后执行）
	 * @return 空字符串表示通过，否则返回错误文案
	 */
	std::string validate()
	{
		// 1. deptIdList 必填且不能为空数组
		if (!deptIdList || deptIdList->empty())
		{
			return ZH_WORDS_GETTER("dept.batch-status.validate.id-list-required");
		}

		// 2. 逐个校验ID格式并查重
		std::unordered_set<std::string> idSet;
		for (auto& strId : *(deptIdList))
		{
			if (!strId)
			{
				return ZH_WORDS_GETTER("dept.batch-status.validate.id-empty");
			}

			std::string idStr(strId->c_str());
			if (idStr.empty())
			{
				return ZH_WORDS_GETTER("dept.batch-status.validate.id-empty");
			}

			for (char c : idStr)
			{
				if (!std::isdigit(static_cast<unsigned char>(c)))
				{
					return std::string(ZH_WORDS_GETTER("dept.batch-status.validate.id-not-digit")) + ": " + idStr;
				}
			}

			if (idSet.find(idStr) != idSet.end())
			{
				return std::string(ZH_WORDS_GETTER("dept.batch-status.validate.id-duplicate")) + ": " + idStr;
			}
			idSet.insert(idStr);
		}

		// 3. targetStatus 必填
		if (!targetStatus)
		{
			return ZH_WORDS_GETTER("dept.batch-status.validate.status-required");
		}

		// 4. 仅允许 0 / 1
		if (targetStatus != 0 && targetStatus != 1)
		{
			return ZH_WORDS_GETTER("dept.batch-status.validate.status-invalid");
		}

		return "";
	}
};

/**
 * Service 传给 DAO 的批量更新载体（内部使用）
 */
class DeptBatchUpdateDTO : public oatpp::DTO
{
	DTO_INIT(DeptBatchUpdateDTO, DTO);

	DTO_FIELD(Vector<String>, idList);
	std::vector<int64_t> numIdList;
	DTO_FIELD(String, statusEnum);
	DTO_FIELD(String, updateBy);
	DTO_FIELD(String, updateTime);

public:
	bool isValid() const
	{
		return !numIdList.empty() &&
			statusEnum && !statusEnum->empty() &&
			updateBy && !updateBy->empty() &&
			updateTime && !updateTime->empty();
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DEPT_BATCH_STATUS_DTO_