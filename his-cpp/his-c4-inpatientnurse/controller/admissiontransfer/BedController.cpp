#include "stdafx.h"
#include "BedController.h"
#include "../../service/admissiontransfer/BedService.h"
#include "dao/common/NurseStationAccessDAO.h"

BedPageJsonVO::Wrapper BedController::execQueryBed(
	const BedQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = BedPageJsonVO::createShared();
    if (!query || !inpatientnurse::validation::validPage(query->pageIndex, query->pageSize)
        || !inpatientnurse::validation::validDate(query->intended_admission_date, false))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
	auto result = BedService().listFree(query, payload.getId());
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
	NurseStationAccessDAO accessDao;
	if (!accessDao.canAccessBed(payload.getId(), dto->location_id.getValue(""))) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
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
