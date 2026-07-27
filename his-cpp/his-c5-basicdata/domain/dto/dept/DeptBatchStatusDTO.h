#pragma once
#ifndef _DEPT_BATCH_STATUS_DTO_
#define _DEPT_BATCH_STATUS_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 前端入参DTO：批量启停科室请求对象
 */
class DeptBatchStatusDTO : public oatpp::DTO
{
	DTO_INIT(DeptBatchStatusDTO, DTO);

	// 批量科室ID集合 - 必填
	DTO_FIELD(Vector<String>, deptIdList);
	DTO_FIELD_INFO(deptIdList) {
		info->description = ZH_WORDS_GETTER("dept.field.deptIdList");
		info->required = true;  // 标记为必填
	}

	// 目标启停状态 0=停用 1=启用 - 必填
	DTO_FIELD(Int32, targetStatus);
	DTO_FIELD_INFO(targetStatus) {
		info->description = ZH_WORDS_GETTER("dept.field.targetStatus");
		info->required = true;  // 标记为必填
	}


	// 负载对象，存放当前登录操作人
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	// 参数合法性校验
    std::string validate()
    {
        // 1. 校验 deptIdList 是否存在
        if (!deptIdList)
        {
            return "deptIdList不能为空";
        }

        // 2. 校验 deptIdList 是否为空数组
        if (deptIdList->empty())
        {
            return "deptIdList至少包含一个科室ID";
        }

        // 3. 校验 deptIdList 去重（避免重复ID）
        std::unordered_set<std::string> idSet;
        for (auto& strId : *(deptIdList))
        {
            std::string idStr(strId->c_str());

            // 3.1 校验是否为空字符串
            if (idStr.empty())
            {
                return "科室ID不能为空字符串";
            }

            // 3.2 校验是否纯数字
            for (char c : idStr)
            {
                if (!isdigit(static_cast<unsigned char>(c)))
                {
                    return "科室ID仅允许数字字符: " + idStr;
                }
            }

            // 3.3 检测重复ID
            if (idSet.find(idStr) != idSet.end())
            {
                return "存在重复的科室ID: " + idStr;
            }
            idSet.insert(idStr);
        }

        // 4. 校验 targetStatus 是否存在
        if (!targetStatus)
        {
            return "targetStatus不能为空";
        }

        // 5. 校验 targetStatus 只允许0或1
        if (targetStatus != 0 && targetStatus != 1)
        {
            return "targetStatus仅支持0(停用)或1(启用)";
        }

        return "";  // 校验通过
    }

    /**
     * 获取去重后的ID列表
     */
    std::unordered_set<std::string> getUniqueIdSet() const
    {
        std::unordered_set<std::string> idSet;
        if (deptIdList)
        {
            for (auto& strId : *(deptIdList))
            {
                idSet.insert(std::string(strId->c_str()));
            }
        }
        return idSet;
    }

};

/**
 * Service传给DAO的批量更新载体DTO（内部使用，不对外接口）
 * 仅包含批量更新SQL必需字段：ID数组、目标状态、操作人、更新时间
 */
class DeptBatchUpdateDTO : public oatpp::DTO
{
	DTO_INIT(DeptBatchUpdateDTO, DTO);

	// 待更新科室ID数组
	DTO_FIELD(Vector<String>, idList);
	// 新增：数字ID数组（DAO查询使用，匹配bigint主键）
	std::vector<int64_t> numIdList;
	// 枚举状态字段
	DTO_FIELD(String, statusEnum);
	// 修改人账号
	DTO_FIELD(String, updateBy);
	// 修改时间字符串
	DTO_FIELD(String, updateTime);

public:
    /**
     * 校验更新DTO是否完整
     */
    bool isValid() const
    {
        return idList != nullptr && !idList->empty() &&
            statusEnum && !statusEnum->empty() &&
            updateBy && !updateBy->empty() &&
            updateTime && !updateTime->empty();
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DEPT_BATCH_STATUS_DTO_