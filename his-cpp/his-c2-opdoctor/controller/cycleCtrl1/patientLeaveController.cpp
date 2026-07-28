#include "stdafx.h"
#include "patientLeaveController.h"
#include "service/cycleCtrl1/patientLeaveService.h"


PatientLeaveJsonVO::Wrapper PatientLeaveController::execLeavePatient(const PatientLeaveQuery::Wrapper& dto, const PayloadDTO& payload)
{
    auto vo = PatientLeaveJsonVO::createShared();

    oatpp::String encounterId = dto->encounterId;
    oatpp::String tenantId = payload.getId();
    if (!payload.getId().empty()) {
        tenantId = oatpp::String(payload.getId().c_str());
    }

    if (!encounterId || encounterId->empty()) {
        vo->code = 10001;
        vo->message = "encounterId is required";
        vo->data = nullptr;
        return vo;
    }

    PatientLeaveService service;
    return service.execLeavePatient(tenantId, encounterId);
}