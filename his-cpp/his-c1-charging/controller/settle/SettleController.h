#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: ovo
 @Date: 2026/07/20

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
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
		ZH_WORDS_GETTER("charging.settle.settle.summary"),
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
