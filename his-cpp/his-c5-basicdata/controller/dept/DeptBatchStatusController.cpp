#include "stdafx.h"
#include "DeptBatchStatusController.h"
#include "../../service/dept/DeptBatchStatusService.h"

DeptBatchStatusJsonVO::Wrapper DeptBatchStatusController::execBatchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto respVo = DeptBatchStatusJsonVO::createShared();

    // 第一步：执行入参自定义校验
    std::string errMsg = dto->validate();
    if (!errMsg.empty())
    {
        respVo->init(RS_PARAMS_INVALID);
        respVo->message = errMsg;
        return respVo;
    }

    // 将当前登录用户载荷绑定到请求DTO内部
    dto->setPayload(&payload);

    // 实例化Service，调用批量启停业务方法
    DeptBatchStatusService batchService;
    BatchResult result = batchService.batchToggleDeptStatus(dto);

    // 根据结果组装响应
    if (result.success)
    {
        respVo->init(RS_SUCCESS);
        if (dto->targetStatus == 1)
        {
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.enable");
        }
        else
        {
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.disable");
        }
    }
    else
    {
        respVo->init(RS_FAIL);
        if (!result.errorMsg.empty())
        {
            respVo->message = result.errorMsg;
        }
        else
        {
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.fail.empty");
        }
    }

    return respVo;
}