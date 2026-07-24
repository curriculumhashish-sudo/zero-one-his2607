#include "stdafx.h"
#include "DeptBatchStatusController.h"

// TODO: Replace with Service layer call when available
// #include "service/dept_batch_status_service.hpp"

DeptBatchStatusJsonVO::Wrapper DeptBatchStatusController::execBatchToggleDeptStatus(
    const DeptBatchStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
    std::string errMsg = dto->validate();
    if (!errMsg.empty())
    {
        auto vo = DeptBatchStatusJsonVO::createShared();
        vo->init(RS_FAIL);
        vo->message = errMsg;
        return vo;
    }

    dto->setPayload(&payload);
    bool opSuccess = true;

    auto respVo = DeptBatchStatusJsonVO::createShared();
    if (opSuccess)
    {
        respVo->init(RS_SUCCESS);
        if (dto->targetStatus == 1)
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.enable");
        else
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.disable");
    }
    else
    {
        respVo->init(RS_FAIL);
        respVo->message = ZH_WORDS_GETTER("dept.batch-status.fail.empty");
    }
    return respVo;
}
