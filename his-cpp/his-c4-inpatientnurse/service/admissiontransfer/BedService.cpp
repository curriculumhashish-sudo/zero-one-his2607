#include "stdafx.h"
#include "BedService.h"
#include "../../dao/admissiontransfer/BedDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

BedPageDTO::Wrapper BedService::listFree(const BedQuery::Wrapper& query)
{
    auto pages = BedPageDTO::createShared();
    pages->pageIndex = query->pageIndex;
    pages->pageSize = query->pageSize;

    BedDAO dao;
    uint64_t count = dao.countFree(query);
    if (count <= 0) {
        return pages;
    }

    pages->total = count;
    pages->calcPages();
    std::list<adm_locationDO> result = dao.selectFreeWithPage(query);
    for (adm_locationDO& sub : result)
    {
        auto dto = BedDTO::createShared();
        ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub,
            id, Id, name, Name, parent_id, Parent_id,
            form_enum, Form_enum, status_enum, Status_enum, organization_id, Organization_id);
        pages->addData(dto);
    }
    return pages;
}

std::string BedService::assignBed(const BedAssignAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
    bkg_bed_reservation_extDO data;
    UuidFacade uf;
    data.setId(uf.genUuid());
    data.setAppointment_id(dto->appointment_id.getValue(""));
    data.setLocation_id(dto->location_id.getValue(""));
    data.setPatient_id(dto->patient_id.getValue(""));
    data.setIntended_admission_date(dto->intended_admission_date.getValue(""));
    data.setCreated_at(SimpleDateTimeFormat::format());
    data.setCreated_by(payload.getUsername());
    data.setIs_deleted(false);
    try {
        BedDAO dao;
        return dao.reserve(data) == 1 ? data.getId() : "";
    }
    catch (...) {
        // Duplicate key (location_id + intended_admission_date) => bed already reserved
        return "";
    }
}
