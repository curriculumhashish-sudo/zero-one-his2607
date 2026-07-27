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

    // 临时测试用：强制使用你在 Navicat 中验证能查到的 tenant/practitioner
    // TODO：排查通过后请改回从鉴权上下文读取
    std::string tenantId = "1";
    std::string practitionerId = "322963602326687744";

    // 调用 Service 获取 DTO
    auto dto = ReceptionPatientService().getRecePatient(query, tenantId, practitionerId);

    // 包装为成功响应
    jvo->success(dto);
    return jvo;
}