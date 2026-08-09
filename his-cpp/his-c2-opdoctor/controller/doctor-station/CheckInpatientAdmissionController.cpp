#include"stdafx.h"
#include"CheckInpatientAdmissionController.h"
#include"../../service/doctor-station/CheckInpatientAdmissionService.h"

CheckInpatientAdmissionJsonVO::Wrapper CheckInpatientAdmissionController::execCheck(const CheckInpatientAdmissionDTO::Wrapper& dto)
{
    auto jvo = CheckInpatientAdmissionJsonVO::createShared();

    if (!dto->encounterId || dto->encounterId.getValue("").empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }

    auto result = CheckInpatientAdmissionService().check(dto);
    if (!result)
    {
        jvo->fail(nullptr);
        return jvo;
    }

    jvo->success(result);
    return jvo;
}
