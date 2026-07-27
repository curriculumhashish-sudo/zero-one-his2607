#pragma once
#ifndef _DEPT_BATCH_STATUS_CONTROLLER_
#define _DEPT_BATCH_STATUS_CONTROLLER_

#include "ApiHelper.h"
#include "domain/vo/NoDataJsonVO.h"
#include "domain/dto/dept/DeptBatchStatusDTO.h"
#include "domain/vo/dept/DeptBatchStatusVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG_DEPT ZH_WORDS_GETTER("dept.tag")

/**
 * DeptBatchStatusController - Batch enable/disable department status.
 */
class DeptBatchStatusController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(DeptBatchStatusController);
public:
    // Batch toggle department status endpoint
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("dept.batch-status.summary"),
        batchToggleDeptStatus,
        DeptBatchStatusJsonVO::Wrapper,
        API_TAG_DEPT,
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/dept/batch-status",
        batchToggleDeptStatus,
        BODY_DTO(DeptBatchStatusDTO::Wrapper, dto),
        execBatchToggleDeptStatus(dto, authObject->getPayload())
    );

private:
    DeptBatchStatusJsonVO::Wrapper execBatchToggleDeptStatus(
        const DeptBatchStatusDTO::Wrapper& dto,
        const PayloadDTO& payload
    );
};

#undef API_TAG_DEPT
#include OATPP_CODEGEN_END(ApiController)
#endif // _DEPT_BATCH_STATUS_CONTROLLER_
