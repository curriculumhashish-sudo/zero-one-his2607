#pragma once
#ifndef _BED_CONTROLLER_H_
#define _BED_CONTROLLER_H_

#include "ApiHelper.h"
#include "domain/dto/bed/BedModifyDTO.h"
#include "domain/dto/bed/BedStatusDTO.h"
#include "domain/vo/bed/BedVO.h"
#include "domain/vo/BaseJsonVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("bed.tag")

class BedController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(BedController);

public:
    // Modify bed
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("bed.modify.summary"),
        modifyBed,
        BedJsonVO::Wrapper,
        API_TAG,
        info->description = "Modify bed basic information by ID.";
        info->addConsumes<BedModifyDTO::Wrapper>("application/json");
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/bed", modifyBed, BODY_DTO(BedModifyDTO::Wrapper, dto), execModifyBed(dto));

    // Remove bed
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("bed.remove.summary"),
        removeBed,
        BooleanJsonVO::Wrapper,
        API_TAG,
        info->description = "Remove bed by ID.";
        API_DEF_ADD_PATH_PARAMS(String, "id", ZH_WORDS_GETTER("bed.field.id"), "322955479021522944", true);
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/bed/{id}", removeBed, PATH(String, id, "id"), execRemoveBed(id));

    // Change bed status
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("bed.status.summary"),
        modifyBedStatus,
        BedJsonVO::Wrapper,
        API_TAG,
        info->description = "Enable or disable bed by statusEnum. ACTIVE means enabled and DISABLED means disabled.";
        info->addConsumes<BedStatusDTO::Wrapper>("application/json");
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/bed/status", modifyBedStatus, BODY_DTO(BedStatusDTO::Wrapper, dto), execModifyBedStatus(dto));

private:
    BedJsonVO::Wrapper execModifyBed(const BedModifyDTO::Wrapper& dto)
    {
        auto data = BedVO::createShared();
        if (dto)
        {
            data->id = dto->id;
            data->name = dto->name;
            data->parentId = dto->parentId;
            data->organizationId = dto->organizationId;
            data->displayOrder = dto->displayOrder;
            data->typeJson = dto->typeJson;
            data->version = dto->version;
        }
        auto result = BedJsonVO::createShared();
        result->success(data);
        return result;
    }

    BooleanJsonVO::Wrapper execRemoveBed(const String& id)
    {
        auto result = BooleanJsonVO::createShared();
        result->success(true);
        return result;
    }

    BedJsonVO::Wrapper execModifyBedStatus(const BedStatusDTO::Wrapper& dto)
    {
        auto data = BedVO::createShared();
        if (dto)
        {
            data->id = dto->id;
            data->statusEnum = dto->statusEnum;
            data->version = dto->version;
        }
        auto result = BedJsonVO::createShared();
        result->success(data);
        return result;
    }
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _BED_CONTROLLER_H_
