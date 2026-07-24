#pragma once

#ifndef _PHARM_CONTROLLER_
#define _PHARM_CONTROLLER_

// 项目统一辅助头文件
#include "ApiHelper.h"
#include "Macros.h"

#include "domain/vo/pharm/PharmVO.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/pharm/PharmQuery.h"
#include "domain/dto/pharm/PharmDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("pharm.tag")

class PharmController : public oatpp::web::server::api::ApiController
{

    API_ACCESS_DECLARE(PharmController);

public:
    PharmController()
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared()) {};

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.statusEnum.modify"),
        modifyPharmStatusEnum,
        PharmPageJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/pharm/statusEnum/modify",
        modifyPharmStatusEnum,
        BODY_DTO(PharmModifyStatusEnumDTO::Wrapper, dto),
        execModifyPharmStatusEnum(
            dto
        )
    );

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.list.query"),
        queryPharmList,
        PharmPageJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/pharm/list/query",
        queryPharmList,
        BODY_DTO(PharmQuery::Wrapper, dto),
        execQueryPharmList(
            dto
        )
    );

private:
    StringJsonVO::Wrapper execModifyPharmStatusEnum(const PharmModifyStatusEnumDTO::Wrapper& dto);
    PharmPageJsonVO::Wrapper execQueryPharmList(const PharmQuery::Wrapper& query);
};

#undef API_TAG

#endif // !_PHARM_CONTROLLER_
