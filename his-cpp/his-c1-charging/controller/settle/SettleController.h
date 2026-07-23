#pragma once

#ifndef _SETTLE_CONTROLLER_
#define _SETTLE_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/DTO/settle/SettleDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("charging.settle.tag")

class SettleController :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(SettleController);
public:
	ENDPOINT_INFO(addregitser) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("yxy.charging.settle.summary3"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		API_DEF_ADD_TAG(API_TAG);
		API_DEF_ADD_PATH_PARAMS(String, "hosp_id", ZH_WORDS_GETTER("yxy.charging.settle.hosp_id"), "", true);
		API_DEF_ADD_PATH_PARAMS(String, "type", ZH_WORDS_GETTER("yxy.charging.settle.type"), "", true);
		API_DEF_ADD_PATH_PARAMS(UInt64, "time", ZH_WORDS_GETTER("yxy.charging.settle.time"), 1, true);
		API_DEF_ADD_PATH_PARAMS(Float64, "total_amount", ZH_WORDS_GETTER("yxy.charging.settle.total_amount"), 0.0, true);
		API_DEF_ADD_PATH_PARAMS(Float64, "reimburse", ZH_WORDS_GETTER("yxy.charging.settle.reimburse"), 0.0, true);
		API_DEF_ADD_PATH_PARAMS(Float64, "self_cost", ZH_WORDS_GETTER("yxy.charging.settle.self_cost"), 0.0, true);
		API_DEF_ADD_PATH_PARAMS(String, "method", ZH_WORDS_GETTER("yxy.charging.settle.method"), 0.0, true);
	
	}
	ENDPOINT(API_M_POST, "/charging/settle/settle", addregitser, BODY_DTO(SettleDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execQuerySettle(dto));
	}
private:
	StringJsonVO::Wrapper execQuerySettle(const SettleDTO::Wrapper& dto);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
