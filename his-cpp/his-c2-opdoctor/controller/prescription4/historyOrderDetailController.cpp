#include "stdafx.h"
#include "historyOrderDetailController.h"
#include "../../service/prescription4/historyOrderDetailService.h"

HistoryOrderDetailPageJsonVO::Wrapper HistoryOrderDetailController::execQueryHistoryOrderDetail(const HistoryOrderDetailQuery::Wrapper& query)
{
    auto jvo = HistoryOrderDetailPageJsonVO::createShared();

    // 参数校验：visitId 必传
    if (!query || !query->visitId || query->visitId->empty()) {
        jvo->fail("visitId is required");
        return jvo;
    }

    // 调用 Service 查询
    HistoryOrderDetailService service;
    auto pageDto = service.listAll(query);

    jvo->success(pageDto);
    return jvo;
}