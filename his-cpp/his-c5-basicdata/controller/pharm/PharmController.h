#pragma once
#ifndef _PHARM_CONTROLLER_H_
#define _PHARM_CONTROLLER_H_

// 项目统一辅助头文件
#include "ApiHelper.h"
#include "Macros.h"
using namespace oatpp;

// 业务层、入参出参头文件
#include "../../service/pharm/PharmService.h"
#include "../../domain/dto/pharm/PharmAddDTO.h"
#include "../../domain/dto/pharm/PharmModifyDTO.h"
#include "../../domain/query/pharm/PharmQuery.h"
#include "../../domain/vo/pharm/PharmPageVO.h"
#include "../../domain/vo/pharm/PharmDetailVO.h"
#include "domain/vo/BaseJsonVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("pharm.tag")

class PharmController : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(PharmController);

public:
    PharmController()
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared()) {}

    // ========== 1. 分页查询列表 ==========
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.list"),
        getPharmList,
        PharmQuery,
        PharmPageJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/pharm/list",
        getPharmList,
        PharmQuery,
        execGetPharmList(query)
    );

    // ========== 2. 新增 ==========
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.add"),
        addPharm,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/pharm",
        addPharm,
        BODY_DTO(PharmAddDTO::Wrapper, dto),
        execAddPharm(dto)
    );

    // ========== 3. 修改 ==========
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.modify"),
        modifyPharm,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/pharm",
        modifyPharm,
        BODY_DTO(PharmModifyDTO::Wrapper, dto),
        execModifyPharm(dto)
    );

    // ========== 4. 删除 ==========
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.remove"),
        removePharm,
        StringJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(
            String,
            "id",
            ZH_WORDS_GETTER("pharm.field.id"),
            "",
            true
        );
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_DEL,
        "/pharm/{id}",
        removePharm,
        PATH(String, id),
        execRemovePharm(id)
    );

    // ========== 5. 切换状态 ==========
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pharm.endpoints.status"),
        updatePharmStatus,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/pharm/status",
        updatePharmStatus,
        BODY_DTO(PharmModifyDTO::Wrapper, dto),
        execUpdatePharmStatus(dto->id.getValue(0), dto->status.getValue(0))
    );

private:
    PharmPageJsonVO::Wrapper execGetPharmList(const PharmQuery::Wrapper& query);
    StringJsonVO::Wrapper execAddPharm(const PharmAddDTO::Wrapper& dto);
    StringJsonVO::Wrapper execModifyPharm(const PharmModifyDTO::Wrapper& dto);
    StringJsonVO::Wrapper execRemovePharm(const String& id);
    StringJsonVO::Wrapper execUpdatePharmStatus(uint64_t id, int status);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PHARM_CONTROLLER_H_