#include "stdafx.h"
#include "HistoryOrderListController.h"
#include "../../service/prescription4/HistoryOrderListService.h"

HistoryOrderListPageJsonVO::Wrapper HistoryOrderListController::execQueryHistoryOrderList(const HistoryOrderListQuery::Wrapper& query)
{
    auto jvo = HistoryOrderListPageJsonVO::createShared();

    // 参数校验：patientId 必传
    if (!query || !query->patientId || query->patientId->empty()) {
        jvo->fail("patientId is required");
        return jvo;
    }

    // 调用 Service 查询
    HistoryOrderListService service;
    auto pageDto = service.listAll(query);

    jvo->success(pageDto);
    return jvo;
}