#include"stdafx.h"
#include"AdmissionController.h"
#include"../../service/cycleCtrl2/AdmissionService.h"

Admission2ResultJsonVO::Wrapper Admission2Controller::execApplyAdmission(const RequestInpatientAdmissionDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = Admission2ResultJsonVO::createShared();

    if (!dto->encounterId || dto->encounterId.getValue("").empty())
    {
        auto err = Admission2ResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "encounterId is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!dto->patientId || dto->patientId.getValue("").empty())
    {
        auto err = Admission2ResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "patientId is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!dto->targetDepartmentId || dto->targetDepartmentId.getValue("").empty())
    {
        auto err = Admission2ResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "targetDepartmentId is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }

    std::string operatorId = payload.getUsername();
    auto result = AdmissionService().applyAdmission(dto, operatorId);
    if (result->status == "SUCCESS")
        jvo->success(result);
    else
        jvo->fail(result);
    return jvo;
}
