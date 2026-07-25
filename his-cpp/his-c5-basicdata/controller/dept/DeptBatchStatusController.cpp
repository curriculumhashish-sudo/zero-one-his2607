#include "stdafx.h"
#include "DeptBatchStatusController.h"
// 引入Service头文件
#include "../../service/dept/DeptBatchStatusService.h"

DeptBatchStatusJsonVO::Wrapper DeptController::execBatchToggleDeptStatus(const DeptBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload)
{
    // 第一步：执行入参自定义校验
    std::string errMsg = dto->validate();
    if (!errMsg.empty())
    {
        auto vo = DeptBatchStatusJsonVO::createShared();
        vo->init(RS_FAIL);
        vo->message = errMsg;
        return vo;
    }

    // 将当前登录用户载荷绑定到请求DTO内部
    dto->setPayload(&payload);

    // 实例化Service，调用批量启停业务方法
    auto deptBatchService = DeptBatchStatusService::getInstance();
    bool opSuccess = deptBatchService->batchToggleDeptStatus(dto);

    // 组装最终返回响应
    auto respVo = DeptBatchStatusJsonVO::createShared();
    if (opSuccess)
    {
        respVo->init(RS_SUCCESS);
        // 根据目标状态，返回不同提示文案：启用成功 / 停用成功
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
        respVo->message = ZH_WORDS_GETTER("dept.batch-status.fail.empty");
    }

    return respVo;
}
