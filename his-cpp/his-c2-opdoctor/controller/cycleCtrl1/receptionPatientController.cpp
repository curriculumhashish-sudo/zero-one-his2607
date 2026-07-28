#include "stdafx.h"
#include "receptionPatientController.h"
#include "service/cycleCtrl1/ReceptionPatientService.h"

RecePatientJsonVO::Wrapper RecePatientController::execQueryRece(const RecePatientQuery::Wrapper& query)
{
    auto jvo = RecePatientJsonVO::createShared();

    // 参数校验
    if (!query || !query->visitTime)
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    //测试
    std::string tenantId = "1";
    std::string practitionerId = "322963602326687744";

    // 调用 Service 获取 DTO
    auto dto = ReceptionPatientService().getRecePatient(query, tenantId, practitionerId);

    // 包装为成功响应
    jvo->success(dto);
    return jvo;
}