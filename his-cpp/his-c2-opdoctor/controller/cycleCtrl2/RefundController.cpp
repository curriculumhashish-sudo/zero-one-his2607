#include"stdafx.h"
#include"RefundController.h"
#include"../../service/cycleCtrl2/RefundService.h"

Refund2DetailJsonVO::Wrapper Refund2Controller::executeQueryRefund(const Refund2Query::Wrapper& query)
{
    auto jvo = Refund2DetailJsonVO::createShared();
    std::string encounterId = query->encounterId.getValue("");
    std::string num = query->num.getValue("");
    if (encounterId.empty() && num.empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    auto result = RefundService().queryRefundList(query);
    jvo->success(result);
    return jvo;
}

Refund2ApplyResultJsonVO::Wrapper Refund2Controller::execApplyRefund(const Refund2ApplyDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = Refund2ApplyResultJsonVO::createShared();
    if (!dto->encounterId || dto->encounterId.getValue("").empty())
    {
        auto err = Refund2ApplyResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "encounter id is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!dto->apply || dto->apply->empty())
    {
        auto err = Refund2ApplyResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "refund item list is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }
    std::string operatorId = payload.getUsername();
    auto result = RefundService().applyRefund(dto, operatorId);
    if (result->status == "SUCCESS")
        jvo->success(result);
    else
        jvo->fail(result);
    return jvo;
}
