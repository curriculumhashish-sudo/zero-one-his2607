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
		API_DEF_ADD_PATH_PARAMS(String, "id_number", ZH_WORDS_GETTER("yxy.charging.register.id_number"), "", true);
		API_DEF_ADD_PATH_PARAMS(String, "name", ZH_WORDS_GETTER("yxy.charging.register.name"), "", true);
		API_DEF_ADD_PATH_PARAMS(UInt32, "phone", ZH_WORDS_GETTER("yxy.charging.register.phone"), 1, false);
		API_DEF_ADD_PATH_PARAMS(String, "sex", ZH_WORDS_GETTER("yxy.charging.register.sex"),"未知", true);
		API_DEF_ADD_PATH_PARAMS(UInt64, "birth_date", ZH_WORDS_GETTER("yxy.charging.register.birth_date"), 1, false);
		API_DEF_ADD_PATH_PARAMS(UInt32, "age", ZH_WORDS_GETTER("yxy.charging.register.age"), 18, true);
		API_DEF_ADD_PATH_PARAMS(String, "oganization", ZH_WORDS_GETTER("yxy.charging.register.oganization"), "", true);
		API_DEF_ADD_PATH_PARAMS(String, "doctor", ZH_WORDS_GETTER("yxy.charging.register.doctor"), "", true);
		API_DEF_ADD_PATH_PARAMS(String, "register_type", ZH_WORDS_GETTER("yxy.charging.register.register_type"), "", true);
		API_DEF_ADD_PATH_PARAMS(String, "cost_type", ZH_WORDS_GETTER("yxy.charging.register.cost_type"), "", true);
	}
	ENDPOINT(API_M_POST, "/charging/register/add_register", addregitser, BODY_DTO(AddRegisterDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execQueryAddRegitser(dto));
	}
private:
	StringJsonVO::Wrapper execQueryAddRegitser(const AddRegisterDTO::Wrapper& dto);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
