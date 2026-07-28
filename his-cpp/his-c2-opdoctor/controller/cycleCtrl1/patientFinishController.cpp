#include "stdafx.h"
#include "patientFinishController.h"
#include "domain/query/cycleCtrl1/patientFinishQuery.h"
#include "service/cycleCtrl1/patientFinishService.h"


PatientFinishJsonVO::Wrapper PatientFinishController::execFinishPatient(const PatientFinishQuery::Wrapper& dto, const PayloadDTO& payload)
{
    auto vo = PatientFinishJsonVO::createShared();

    // 使用请求体中的 encounterId
    oatpp::String encounterId = dto->encounterId;
    // 从 payload 取 tenantId
    oatpp::String tenantId = nullptr;
    if (!payload.getId().empty()) {
        tenantId = oatpp::String(payload.getId().c_str());
    }

    // 参数校验
    if (!encounterId || encounterId->empty())
    {
        vo->init(nullptr, RS_PARAMS_INVALID); 
        return vo;
    }

    PatientFinishService service;
    return service.execFinishPatient(tenantId, encounterId);
}