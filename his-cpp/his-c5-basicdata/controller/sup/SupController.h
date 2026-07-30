#pragma once

#ifndef _SUP_CONTROLLER_
#define _SUP_CONTROLLER_

// 项目统一辅助头文件
#include "ApiHelper.h"
#include "Macros.h"

#include "domain/vo/sup/SupVO.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/query/sup/SupQuery.h"
#include "domain/dto/sup/SupDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("sup.tag")

class SupController : public oatpp::web::server::api::ApiController
{

    API_ACCESS_DECLARE(SupController);

public:
    SupController()
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared()) {};

    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.name.query"),
        querySupName,
        SupPageJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/sup/statusEnum/modify",
        querySupName,
        BODY_DTO(SupNameDTO::Wrapper, dto),
        execQuerySupName(
            dto
        )
    );

    

private:
    StringJsonVO::Wrapper execQuerySupName(const SupNameDTO::Wrapper& dto);
};

#undef API_TAG

#endif // !_SUP_CONTROLLER_
