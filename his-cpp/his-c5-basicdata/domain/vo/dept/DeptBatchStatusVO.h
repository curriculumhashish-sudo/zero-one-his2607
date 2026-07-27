#pragma once

#ifndef _DEPT_BATCH_STATUS_VO_H_
#define _DEPT_BATCH_STATUS_VO_H_

#include "domain/vo/NoDataJsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Batch status response VO - returns status + message without data field.
 * Inherits NoDataJsonVO for scenarios that only need success/failure indication.
 */
class DeptBatchStatusJsonVO : public NoDataJsonVO
{
    DTO_INIT(DeptBatchStatusJsonVO, NoDataJsonVO);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _DEPT_BATCH_STATUS_VO_H_
