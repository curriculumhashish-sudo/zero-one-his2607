#pragma once

#ifndef _DEPT_BATCH_STATUS_VO_H_
#define _DEPT_BATCH_STATUS_VO_H_

#include "domain/vo/NoDataJsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 批量启停响应VO（仅返回 code / message，无 data）
 */
class DeptBatchStatusJsonVO : public NoDataJsonVO
{
	DTO_INIT(DeptBatchStatusJsonVO, NoDataJsonVO);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _DEPT_BATCH_STATUS_VO_H_