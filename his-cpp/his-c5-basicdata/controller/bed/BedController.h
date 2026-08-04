#pragma once
#ifndef _BED_CONTROLLER_H_
#define _BED_CONTROLLER_H_

#include "ApiHelper.h"
#include "ServerInfo.h"
#include "domain/dto/bed/BedModifyDTO.h"
#include "domain/dto/bed/BedStatusDTO.h"
#include "domain/vo/bed/BedVO.h"
#include "domain/vo/BaseJsonVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("ward.tag")

class BedController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(BedController);

public:
    // Modify bed
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("ward.bed-modify.summary"),
        modifyBed,
        BedJsonVO::Wrapper,
        API_TAG,
        info->description = "Modify bed basic information by ID.";
        info->addConsumes<BedModifyDTO::Wrapper>("application/json");
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/bed", modifyBed, BODY_DTO(BedModifyDTO::Wrapper, dto), execModifyBed(dto, authObject->getPayload()));

    // Remove bed
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("ward.bed-remove.summary"),
        removeBed,
        BooleanJsonVO::Wrapper,
        API_TAG,
        info->description = "Remove bed by ID.";
        API_DEF_ADD_PATH_PARAMS(String, "id", ZH_WORDS_GETTER("ward.field.bedId"), "322955479021522944", true);
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/bed/{id}", removeBed, PATH(String, id, "id"), execRemoveBed(id, authObject->getPayload()));

    // Change bed status
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("ward.bed-status.summary"),
        modifyBedStatus,
        BedJsonVO::Wrapper,
        API_TAG,
        info->description = "Enable or disable bed by statusEnum. ACTIVE means enabled and DISABLED means disabled.";
        info->addConsumes<BedStatusDTO::Wrapper>("application/json");
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/bed/status", modifyBedStatus, BODY_DTO(BedStatusDTO::Wrapper, dto), execModifyBedStatus(dto, authObject->getPayload()));

private:
    BedJsonVO::Wrapper execModifyBed(
        const BedModifyDTO::Wrapper& dto,
        const PayloadDTO& payload);

    BooleanJsonVO::Wrapper execRemoveBed(
        const String& id,
        const PayloadDTO& payload);

    BedJsonVO::Wrapper execModifyBedStatus(
        const BedStatusDTO::Wrapper& dto,
        const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _BED_CONTROLLER_H_
