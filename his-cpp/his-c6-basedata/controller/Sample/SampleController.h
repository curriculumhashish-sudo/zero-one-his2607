#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/22 17:39:36

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
#ifndef _SAMPLECONTROLLER_H_
#define _SAMPLECONTROLLER_H_

#include "domain/query/Sample/SampleQuery.h"
#include "domain/query/Sample/SampleNameQuery.h"
#include "domain/query/Sample/SampleRemoveQuery.h"

#include "domain/dto/Sample/SampleDTO.h"
#include "domain/dto/Sample/SampleAddDTO.h"
#include "domain/dto/Sample/SampleEditDTO.h"
#include "domain/dto/Sample/SampleStatusDTO.h"

#include "domain/vo/BaseJsonVO.h"

#include "domain/vo/Sample/SampleJsonVO.h"
#include "domain/vo/Sample/SampleAddVO.h"
#include "domain/vo/Sample/SampleEditVO.h"
#include "domain/vo/Sample/SampleRemoveVO.h"
#include "domain/vo/Sample/SampleStatusVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("sample.tags.tag")

class SampleController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(SampleController);
public:
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("sample.select-name.summary"), listSampleName, SampleNameQuery, SampleListJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/sample/select-name", listSampleName, SampleNameQuery, executeListSampleName(query));

	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("sample.list-page.summary"), listSamplePage, SampleQuery, SamplePageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/sample/list-page", listSamplePage, SampleQuery, executeListSamplePage(query));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.add-sample.summary"), addSample, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/sample/add-sample", addSample, BODY_DTO(SampleAddDTO::Wrapper, dto), executeAddSample(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.edit-sample.summary"), editSample, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/sample/edit-sample", editSample, BODY_DTO(SampleEditDTO::Wrapper, dto), executeEditSample(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.remove-sample.summary"), removeSample, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/sample/remove-sample", removeSample, BODY_DTO(SampleRemoveQuery::Wrapper, dto), executeRemoveSample(dto));

	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.switch-status.summary"), switchSampleStatus, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/sample/switch-status", switchSampleStatus, BODY_DTO(SampleStatusDTO::Wrapper, dto), executeSwitchSampleStatus(dto));

private:
	SampleListJsonVO::Wrapper executeListSampleName(const SampleNameQuery::Wrapper& query);
	SamplePageJsonVO::Wrapper executeListSamplePage(const SampleQuery::Wrapper& query);
	StringJsonVO::Wrapper executeAddSample(const SampleAddDTO::Wrapper& dto);
	StringJsonVO::Wrapper executeEditSample(const SampleEditDTO::Wrapper& dto);
	StringJsonVO::Wrapper executeRemoveSample(const SampleRemoveQuery::Wrapper& dto);
	StringJsonVO::Wrapper executeSwitchSampleStatus(const SampleStatusDTO::Wrapper& dto);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _SAMPLECONTROLLER_H_
