/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/08/01

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
#ifndef _NURSEOPERATEDTO_H_
#define _NURSEOPERATEDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 执行操作 DTO
class NurseOperateDTO2 : public oatpp::DTO
{
	DTO_INIT(NurseOperateDTO2, DTO);

	// 就诊ID（必填）
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("nurse2.field.encounter_id"), true);
	// 待执行的项目ID，对应 wkf_service_request.id（必填）
	API_DTO_FIELD_REQUIRE(String, itemId, ZH_WORDS_GETTER("nurse2.field.item_id"), true);
	// 目标执行状态（选填，默认 COMPLETED）
	API_DTO_FIELD_REQUIRE(String, state, ZH_WORDS_GETTER("nurse2.field.state"), false);
	// 执行次数（选填）
	API_DTO_FIELD_REQUIRE(Int32, amount, ZH_WORDS_GETTER("nurse2.field.amount"), false);
	// 乐观锁版本号（选填）
	API_DTO_FIELD_REQUIRE(Int32, version, ZH_WORDS_GETTER("nurse2.field.version"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_NURSEOPERATEDTO_H_
