#pragma once
#ifndef _BED_CONTROLLER_
#define _BED_CONTROLLER_
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/admissiontransfer/BedQuery.h"
#include "domain/dto/admissiontransfer/BedAssignDTO.h"
#include "domain/vo/admissiontransfer/BedVO.h"
#include "controller/RequestValidation.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("admissiontransfer.tag")

class BedController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(BedController);
public:
    // Query available beds.
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("bed.get.summary"), queryBed,
        BedPageJsonVO::Wrapper, API_TAG,
        API_DEF_QUERY_PARAM_BUILD(BedQuery);
		info->queryParams["intended_admission_date"].addExample("default", String("2026-07-24"));
        info->queryParams["status_enum"].addExample("default", String("ENABLE"));
        info->queryParams["organization_id"].addExample("default", String("327170091059744768"));
        info->queryParams["parent_id"].addExample("default", String("334526584536674304"));
        info->responses[Status::CODE_200].addExample(
            "success", BedPageJsonVO::createShared());
    )
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/bed", queryBed, BedQuery,
		execQueryBed(query, authObject->getPayload()));

    // Assign a bed.
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("bed.assign.summary"), assignBed,
        StringJsonVO::Wrapper, API_TAG,
        info->body.addExample("default", BedAssignAddDTO::createExample());
        info->responses[Status::CODE_200].addExample(
            "success", StringJsonVO::createShared());
    )
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/bed/assign", assignBed, C4_REQUIRED_BODY_DTO(BedAssignAddDTO::Wrapper, dto), execAssignBed(dto, authObject->getPayload()));
private:
	BedPageJsonVO::Wrapper execQueryBed(
		const BedQuery::Wrapper& query, const PayloadDTO& payload);
    StringJsonVO::Wrapper  execAssignBed(const BedAssignAddDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif // !_BED_CONTROLLER_
