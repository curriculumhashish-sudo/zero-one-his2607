#include"stdafx.h"
#include"PrescriptionController.h"
#include"../../service/cycleCtrl2/PrescriptionService.h"

Prescription2DetailJsonVO::Wrapper Prescription2Controller::executeQueryPrescription(const Prescription2Query::Wrapper& query)
{
    auto jvo = Prescription2DetailJsonVO::createShared();
    std::string encounterId = query->encounterId.getValue("");
    std::string num = query->num.getValue("");
    if (encounterId.empty() && num.empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    std::string type = query->type.getValue("CURRENT_ENCOUNTER");
    if (type != "today" && type != "history" &&
        type != "CURRENT_ENCOUNTER" && type != "HISTORY")
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    if (type == "CURRENT_ENCOUNTER") type = "today";
    if (type == "HISTORY") type = "history";
    query->type = type;
    auto result = PrescriptionService().queryPrescriptions(query);
    jvo->success(result);
    return jvo;
}
