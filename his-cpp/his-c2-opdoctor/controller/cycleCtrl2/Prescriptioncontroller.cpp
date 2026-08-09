#include"stdafx.h"
#include"PrescriptionController.h"
#include"../../service/cycleCtrl2/PrescriptionService.h"

Prescription2DetailJsonVO::Wrapper PrescriptionController2::executeQueryPrescription(const Prescription2Query::Wrapper& query)
{
    auto jvo = Prescription2DetailJsonVO::createShared();
    std::string encounterId = query->encounterId.getValue("");
    std::string num = query->num.getValue("");
    if (encounterId.empty() && num.empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    std::string type = query->type.getValue("today");
    if (type != "today" && type != "history")
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    auto result = PrescriptionService().queryPrescriptions(query);
    jvo->success(result);
    return jvo;
}
