#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 17:39:36

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
#include "domain/query/sample/SampleFullSelectQuery.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/sample/SampleFullSelectJsonVO.h"
#include "domain/dto/sample/SampleAddDTO.h"
#include "domain/dto/sample/SampleEditDTO.h"
#include "domain/dto/sample/SampleStatusDTO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("sample.tag")

class SampleController : public oatpp::web::server::api::ApiController
{
	// 添加访问定义
	API_ACCESS_DECLARE(SampleController);
public:

	/**
	 * 1. 获取样本名称下拉列表（简易选择）
	 */
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("sample.select-name.summary"), listSampleName, SampleFullSelectJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/sample/list-name", listSampleName, SampleFullSelectQuery, executeListSampleName(query));

	/**
	 * 2. 获取样本分页列表（多条件+分页）
	 */
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("sample.list-page.summary"), listSamplePage, SampleFullSelectJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/sample/list-page", listSamplePage, SampleFullSelectQuery, executeListSamplePage(query));

	/**
	 * 3. 新增样本
	 */
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.add.summary"), addSample, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/sample/add", addSample, BODY_DTO(SampleAddDTO::Wrapper, dto), executeAddSample(dto));

	/**
	 * 4. 修改样本
	 */
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.edit.summary"), editSample, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/sample/edit", editSample, BODY_DTO(SampleEditDTO::Wrapper, dto), executeEditSample(dto));

	/**
	 * 5. 删除样本（单条id）
	 */
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.remove.summary"), removeSample, StringJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("sample.field.id"), "", true);
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_DELETE, "/sample/remove", removeSample, QUERY(String, id), executeRemoveSample(id));

	/**
	 * 6. 启用/停用样本（批量修改状态）
	 */
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.switch-status.summary"), switchSampleStatus, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/sample/switch-status", switchSampleStatus, BODY_DTO(SampleStatusDTO::Wrapper, dto), executeSwitchSampleStatus(dto));

private:
	// 1. 获取样本名称下拉
	SampleFullSelectJsonVO::Wrapper executeListSampleName(const SampleFullSelectQuery::Wrapper& query);
	// 2. 样本分页列表
	SampleFullSelectJsonVO::Wrapper executeListSamplePage(const SampleFullSelectQuery::Wrapper& query);
	// 3. 新增样本
	StringJsonVO::Wrapper executeAddSample(const SampleAddDTO::Wrapper& dto);
	// 4. 修改样本
	StringJsonVO::Wrapper executeEditSample(const SampleEditDTO::Wrapper& dto);
	// 5. 删除样本
	StringJsonVO::Wrapper executeRemoveSample(const String& id);
	// 6. 启用/停用样本
	StringJsonVO::Wrapper executeSwitchSampleStatus(const SampleStatusDTO::Wrapper& dto);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // _SAMPLECONTROLLER_H_