#include "stdafx.h"
#include "DeptBatchStatusController.h"
<<<<<<< HEAD
// 引入Service头文件
#include "../../service/dept/DeptBatchStatusService.h"
=======

// TODO: Replace with Service layer call when available
// #include "service/dept_batch_status_service.hpp"
>>>>>>> 9c70232bf8f9e68687532bbbc58b5f56c13b0ce0

DeptBatchStatusJsonVO::Wrapper DeptBatchStatusController::execBatchToggleDeptStatus(
    const DeptBatchStatusDTO::Wrapper& dto,
    const PayloadDTO& payload)
{
<<<<<<< HEAD
    // 第一步：执行入参自定义校验
=======
>>>>>>> 9c70232bf8f9e68687532bbbc58b5f56c13b0ce0
    std::string errMsg = dto->validate();
    if (!errMsg.empty())
    {
        auto vo = DeptBatchStatusJsonVO::createShared();
        vo->init(RS_FAIL);
        vo->message = errMsg;
        return vo;
    }

<<<<<<< HEAD
    // 将当前登录用户载荷绑定到请求DTO内部
    dto->setPayload(&payload);

    // 实例化Service，调用批量启停业务方法
    DeptBatchStatusService batchService;
    bool opSuccess = batchService.batchToggleDeptStatus(dto);

    // 组装最终返回响应
=======
    dto->setPayload(&payload);
    bool opSuccess = true;

>>>>>>> 9c70232bf8f9e68687532bbbc58b5f56c13b0ce0
    auto respVo = DeptBatchStatusJsonVO::createShared();
    if (opSuccess)
    {
        respVo->init(RS_SUCCESS);
<<<<<<< HEAD
        // 根据目标状态，返回不同提示文案：启用成功 / 停用成功
        if (dto->targetStatus == 1)
        {
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.enable");
        }
        else
        {
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.disable");
        }
=======
        if (dto->targetStatus == 1)
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.enable");
        else
            respVo->message = ZH_WORDS_GETTER("dept.batch-status.success.disable");
>>>>>>> 9c70232bf8f9e68687532bbbc58b5f56c13b0ce0
    }
    else
    {
        respVo->init(RS_FAIL);
        respVo->message = ZH_WORDS_GETTER("dept.batch-status.fail.empty");
    }
<<<<<<< HEAD

=======
>>>>>>> 9c70232bf8f9e68687532bbbc58b5f56c13b0ce0
    return respVo;
}
