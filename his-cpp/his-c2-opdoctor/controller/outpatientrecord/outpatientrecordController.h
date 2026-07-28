#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2026/07/28

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
#ifndef _outpatientrecordCONTROLLER_H_
#define _outpatientrecordCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "../../domain/dto/outpatientrecord/outpatientrecordSaveDTO.h"
#include "../../service/outpatientrecord/OutpatientrecordService.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("outpatientrecord.tags.tag")

class outpatientrecordController
	: public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(outpatientrecordController);

public:

	// Save outpatient medical record
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("outpatientrecord.save.summary"),
		saveoutpatientrecord,
		StringJsonVO::Wrapper,
		API_TAG);

	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/outpatientrecord/save",
		saveoutpatientrecord,
		BODY_DTO(outpatientrecordSaveDTO::Wrapper, dto),
		execSaveoutpatientrecord(dto, authObject->getPayload()));

private:
	// Business service for outpatient record operations
	OutpatientrecordService service;

	StringJsonVO::Wrapper
		execSaveoutpatientrecord(
			const outpatientrecordSaveDTO::Wrapper& dto,
			const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif