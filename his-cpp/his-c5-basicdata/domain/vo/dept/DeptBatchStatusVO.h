#pragma once

#ifndef _DEPT_BATCH_STATUS_VO_H_
#define _DEPT_BATCH_STATUS_VO_H_

#include "../../../../lib-oatpp/include/domain/vo/NoDataJsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 批量启停科室无数据返回VO，仅返回状态码与提示信息
 * 继承NoDataJsonVO，无data字段，适配仅返回成功/失败布尔判断的场景
 */
class DeptBatchStatusJsonVO : public NoDataJsonVO
{
	DTO_INIT(DeptBatchStatusJsonVO, NoDataJsonVO);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _DEPT_BATCH_STATUS_VO_H_