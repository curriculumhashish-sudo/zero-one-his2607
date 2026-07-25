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

	// 批量科室ID集合
	DTO_FIELD(Vector<Int64>, deptIdList);
	DTO_FIELD_INFO(deptIdList) {
		info->description = ZH_WORDS_GETTER("dept.field.deptIdList");
	}

	// 目标启停状态 0=停用 1=启用
	DTO_FIELD(Int32, targetStatus);
	DTO_FIELD_INFO(targetStatus) {
		info->description = ZH_WORDS_GETTER("dept.field.targetStatus");
	}

	// 负载对象，存放当前登录操作人
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	// 参数合法性校验
	std::string validate()
	{
		// 科室ID列表非空校验
		if (!deptIdList || deptIdList->empty())
			return "deptIdList invalidate：科室ID列表不能为空";
		// 状态值只能是0或1
		if (!targetStatus || (targetStatus != 0 && targetStatus != 1))
			return "targetStatus invalidate：状态仅支持0(停用)、1(启用)";
		return "";
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
	DTO_FIELD(Vector<Int64>, idList);
	// 目标启用状态
	DTO_FIELD(Int32, enableStatus);
	// 修改人账号
	DTO_FIELD(String, updateBy);
	// 修改时间字符串
	DTO_FIELD(String, updateTime);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DEPT_BATCH_STATUS_DTO_