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
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/Sample/SampleJsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("sample.tag")

class SampleController : public oatpp::web::server::api::ApiController
{
	// 添加访问定义
	API_ACCESS_DECLARE(SampleController);
public:
	// 定义查询样本名称列表接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("sample.select-name.summary"), listSampleName, SampleQuery, SampleJsonVO::Wrapper, API_TAG);
	// 定义查询样本名称列表接口端点
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/sample/select-name", listSampleName, SampleQuery, executeListSampleName(query));

	// 定义查询样本分页列表接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("sample.list-page.summary"), listSamplePage, SampleQuery, SampleJsonVO::Wrapper, API_TAG);
	// 定义查询样本分页列表接口端点
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/sample/list-page", listSamplePage, SampleQuery, executeListSamplePage(query));

	// 定义新增样本信息接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.add-sample.summary"), addSample, StringJsonVO::Wrapper, API_TAG);
	// 定义新增样本信息接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/sample/add-sample", addSample, BODY_DTO(SampleAddDTO::Wrapper, dto), executeAddSample(dto));

	// 定义修改样本信息接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.edit-sample.summary"), editSample, StringJsonVO::Wrapper, API_TAG);
	// 定义修改样本信息接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/sample/edit-sample", editSample, BODY_DTO(SampleEditDTO::Wrapper, dto), executeEditSample(dto));

	// 定义删除样本信息接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.remove-sample.summary"), removeSample, StringJsonVO::Wrapper, API_TAG);
	// 定义删除样本信息接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/sample/remove-sample", removeSample, BODY_DTO(SampleRemoveQuery::Wrapper, dto), executeRemoveSample(dto));

	// 定义样本启停用接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("sample.switch-status.summary"), switchSampleStatus, StringJsonVO::Wrapper, API_TAG);
	// 定义样本启停用接口端点
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/sample/switch-status", switchSampleStatus, BODY_DTO(SampleStatusDTO::Wrapper, dto), executeSwitchSampleStatus(dto));

private:
	// 查询样本名称列表
	SampleJsonVO::Wrapper executeListSampleName(const SampleQuery::Wrapper& query);
	// 查询样本分页列表
	SampleJsonVO::Wrapper executeListSamplePage(const SampleQuery::Wrapper& query);
	// 新增样本信息
	StringJsonVO::Wrapper executeAddSample(const SampleAddDTO::Wrapper& dto);
	// 修改样本信息
	StringJsonVO::Wrapper executeEditSample(const SampleEditDTO::Wrapper& dto);
	// 删除样本信息
	StringJsonVO::Wrapper executeRemoveSample(const SampleRemoveQuery::Wrapper& dto);
	// 样本启停用状态切换
	StringJsonVO::Wrapper executeSwitchSampleStatus(const SampleStatusDTO::Wrapper& dto);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // _SAMPLECONTROLLER_H_