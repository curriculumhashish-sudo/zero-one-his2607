#pragma once

#ifndef  _DRUGCONTROLLER_H_
#define _DRUGCONTROLLER_H_
#include "domain/vo/BaseJsonVO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)
using namespace oatpp;
class DrugController : public oatpp::web::server::api::ApiController
{
	   API_ACCESS_DECLARE();
};
#include OATPP_CODEGEN_END(ApiController)

#endif 