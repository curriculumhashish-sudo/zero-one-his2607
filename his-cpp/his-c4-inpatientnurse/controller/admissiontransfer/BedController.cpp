#include "stdafx.h"
#include "BedController.h"
#include "../../service/admissiontransfer/BedService.h"

BedPageJsonVO::Wrapper BedController::execQueryBed(const BedQuery::Wrapper& query)
{
    auto result = BedService().listFree(query);
    auto jvo = BedPageJsonVO::createShared();
    jvo->success(result);
    return jvo;
}

StringJsonVO::Wrapper BedController::execAssignBed(const BedAssignAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = StringJsonVO::createShared();

    std::string errmsg = dto->validate();
    if (errmsg != "") {
        jvo->init(errmsg, RS_PARAMS_INVALID);
        return jvo;
    }

    std::string id = BedService().assignBed(dto, payload);
    if (id != "") {
        jvo->success(id);
    }
    else {
        jvo->fail("bed already reserved");
    }
    return jvo;
}
