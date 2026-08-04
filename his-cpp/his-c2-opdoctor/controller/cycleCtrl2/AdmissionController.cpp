#include"stdafx.h"
#include"AdmissionController.h"
#include"../../service/cycleCtrl2/AdmissionService.h"

Admission2ResultJsonVO::Wrapper Admission2Controller::executeApplyAdmission(const Admission2Query::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = Admission2ResultJsonVO::createShared();

    if (!query->num || query->num.getValue("").empty())
    {
        auto err = Admission2ResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "patient number is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!query->admissionDepartment || query->admissionDepartment.getValue("").empty())
    {
        auto err = Admission2ResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "admission department is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }
    if (!query->admissionDiagnosis || query->admissionDiagnosis.getValue("").empty())
    {
        auto err = Admission2ResultDTO::createShared();
        err->status = "FAILED";
        err->statusName = "admission diagnosis is required";
        jvo->init(err, RS_PARAMS_INVALID);
        return jvo;
    }

    std::string operatorId = payload.getUsername();
    auto result = AdmissionService().applyAdmission(query, operatorId);
    if (result->status == "SUCCESS")
        jvo->success(result);
    else
        jvo->fail(result);
    return jvo;
}
