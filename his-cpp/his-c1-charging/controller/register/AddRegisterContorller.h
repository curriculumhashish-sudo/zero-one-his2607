#pragma once

#ifndef _ADD_REGISTER_CONTROLLER_
#define _ADD_REGISTER_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/DTO/register/AddRegisterDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)


#define API_TAG ZH_WORDS_GETTER("charging.register.tag")

class AddRegisterContorller :public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(AddRegisterContorller);
public:
	ENDPOINT_INFO(addregitser) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("yxy.charging.register.summary3"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		API_DEF_ADD_TAG(API_TAG);
	}
	ENDPOINT(API_M_POST, "/charging/register/add_register", addregitser, BODY_DTO(AddRegisterDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execQueryAddRegitser(dto, authObject->getPayload()));
	}
private:
	StringJsonVO::Wrapper execQueryAddRegitser(const AddRegisterDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
