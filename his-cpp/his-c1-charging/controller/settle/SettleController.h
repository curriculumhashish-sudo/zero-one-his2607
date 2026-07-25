#pragma once

#ifndef _SETTLE_CONTROLLER_H_
#define _SETTLE_CONTROLLER_H_

#include "domain/dto/settle/SettleDTO.h"
#include "domain/vo/settle/SettleVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("charging.settle.tag")

class SettleController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(SettleController);

public:
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("ovo.charging.settle.Settle.summary"),
		settle,
		SettleJsonVO::Wrapper,
		API_TAG);

	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/charging/settle/settlement",
		settle,
		BODY_DTO(SettleRequestDTO::Wrapper, dto),
		execSettle(dto));

private:
	SettleJsonVO::Wrapper execSettle(const SettleRequestDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_SETTLE_CONTROLLER_H_
