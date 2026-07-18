#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/18 19:31:13

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
#ifndef _RENTCONTROLLER_H_
#define _RENTCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/rent/RentQuery.h"
#include "domain/vo/rent/RentVO.h"
#include "domain/dto/rent/RentModifyDTO.h"
#include "domain/dto/rent/RentAddDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("rent.tags.tag")

class RentController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(RentController);
public: // 定义接口
	// 3.1 定义分页查询租户接口描述
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("rent.query.summary"), queryRent, RentQuery, RentDetailPageJsonVO::Wrapper, API_TAG);
	// 3.2 定义分页查询租户接口处理
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/rent/query", queryRent, RentQuery, executeQueryRent(query));

	// 3.1 定义新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("rent.add.summary"), addRent, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/rent/add", addRent, BODY_DTO(RentAddDTO::Wrapper, dto), execAddRent(dto, authObject->getPayload()));

	// 3.1 定义修改接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("rent.modify.summary"), modifyRent, StringJsonVO::Wrapper, API_TAG);
	// 3.2 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/rent/modify", modifyRent, BODY_DTO(RentModifyDTO::Wrapper, dto), execModifyRent(dto, authObject->getPayload()));

	// 3.1 定义删除接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("rent.remove.summary"), removeSample, BooleanJsonVO::Wrapper, API_TAG);
	// 3.2 定义删除接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/rent/{id}", removeSample, PATH(String, id), execRemoveRent(id));
private: // 定义接口执行函数
	// 3.3 定义分页查询租户接口执行函数
	RentDetailPageJsonVO::Wrapper executeQueryRent(const RentQuery::Wrapper& query);
	// 3.3 定义新增接口执行函数
	StringJsonVO::Wrapper execAddRent(const RentAddDTO::Wrapper& dto, const PayloadDTO& payload);
	// 3.3 定义修改接口执行函数
	StringJsonVO::Wrapper execModifyRent(const RentModifyDTO::Wrapper& dto, const PayloadDTO& payload);
	// 3.3 定义删除接口执行函数
	BooleanJsonVO::Wrapper execRemoveRent(const String& id);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_RENTCONTROLLER_H_