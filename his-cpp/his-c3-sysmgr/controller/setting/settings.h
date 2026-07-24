#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author:
 @Date: 2026/07/21

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

#include "OatppMacros.h"
#include "ApiHelper.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/setting/SettingAddDTO.h"
#include "domain/dto/setting/SettingModifyDTO.h"
#include "domain/dto/setting/SettingRemoveDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("setting.tag")

class SettingController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(SettingController);

public:
	
	ENDPOINT_INFO(addItem) {
		API_DEF_ADD_AUTH();
		info->summary = ZH_WORDS_GETTER("setting.add.summary");
		API_DEF_ADD_RSP_JSON_WRAPPER(Uint64JsonVO);
		API_DEF_ADD_TAG(API_TAG);
	}

	ENDPOINT_INFO(modifyItem) {
		API_DEF_ADD_AUTH();
		info->summary = ZH_WORDS_GETTER("setting.modify.summary");
		API_DEF_ADD_RSP_JSON_WRAPPER(Uint64JsonVO);
		API_DEF_ADD_TAG(API_TAG);
	}

	ENDPOINT_INFO(removeItem) {
		API_DEF_ADD_AUTH();
		info->summary = ZH_WORDS_GETTER("setting.remove.summary");
		API_DEF_ADD_RSP_JSON_WRAPPER(Uint64JsonVO);
		API_DEF_ADD_TAG(API_TAG);
	}

	//新增
	ENDPOINT(API_M_POST, "/setting/add-item", addItem,
	         BODY_DTO(SettingAddDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execAdd(dto));
	}
	//更新
	ENDPOINT(API_M_PUT, "/setting/modify-item", modifyItem,
	         BODY_DTO(SettingModifyDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execModify(dto));
	}
	//删除
	ENDPOINT(API_M_DEL, "/setting/remove-item", removeItem,
	         BODY_DTO(SettingRemoveDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execRemove(dto));
	}

private:
	// 执行函数
	Uint64JsonVO::Wrapper execAdd(const SettingAddDTO::Wrapper& dto) {
		auto vo = Uint64JsonVO::createShared();
		vo->success(1);
		return vo;
	}
	Uint64JsonVO::Wrapper execModify(const SettingModifyDTO::Wrapper& dto) {
		auto vo = Uint64JsonVO::createShared();
		vo->success(1);
		return vo;
	}
	Uint64JsonVO::Wrapper execRemove(const SettingRemoveDTO::Wrapper& dto) {
		auto vo = Uint64JsonVO::createShared();
		vo->success(1);
		return vo;
	}
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
