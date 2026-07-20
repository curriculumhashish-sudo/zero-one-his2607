#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/19 00:00:00

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
#ifndef _TREATMENTCONTROLLER_H_
#define _TREATMENTCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/treatment/TreatmentVO.h"
#include "domain/dto/treatment/TreatmentAddDTO.h"
#include "domain/dto/treatment/TreatmentModifyDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("treatment.tags.tag")

class TreatmentController : public oatpp::web::server::api::ApiController
{
	// 权限相关声明
	API_ACCESS_DECLARE(TreatmentController);
public: // 接口定义
	// 获取诊疗详情：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.query-one.summary"), queryOneTreatment,
		TreatmentDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("treatment.field.id"), "184000000000000001", true););
	// 获取诊疗详情：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/treatment/query-one", queryOneTreatment, QUERY(String, id), execQueryOne(id));

	// 新增诊疗：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.add.summary"), addTreatment, StringJsonVO::Wrapper, API_TAG);
	// 新增诊疗：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/treatment/add-treatment", addTreatment,
		BODY_DTO(TreatmentAddDTO::Wrapper, dto), execAddTreatment(dto, authObject->getPayload()));

	// 修改诊疗：接口信息
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("treatment.modify.summary"), modifyTreatment, StringJsonVO::Wrapper, API_TAG);
	// 修改诊疗：接口绑定
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/treatment/modify-treatment", modifyTreatment,
		BODY_DTO(TreatmentModifyDTO::Wrapper, dto), execModifyTreatment(dto, authObject->getPayload()));
private: // 接口执行函数
	// 获取诊疗详情：执行函数
	TreatmentDetailJsonVO::Wrapper execQueryOne(const String& id);
	// 新增诊疗：执行函数
	StringJsonVO::Wrapper execAddTreatment(const TreatmentAddDTO::Wrapper& dto, const PayloadDTO& payload);
	// 修改诊疗：执行函数
	StringJsonVO::Wrapper execModifyTreatment(const TreatmentModifyDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_TREATMENTCONTROLLER_H_
