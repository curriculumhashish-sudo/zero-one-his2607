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
#ifndef _GET_SETTLE_HISTORY_QUERY_H_
#define _GET_SETTLE_HISTORY_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetSettleHistoryQuery : public PageQuery
{
	DTO_INIT(GetSettleHistoryQuery, PageQuery);

	// 患者主键。
	API_DTO_FIELD(String, patientId,
		ZH_WORDS_GETTER("ovo.charging.settle.GetSettleHistoryQuery.patientId.description"),
		true, "329874660197928960");

	// 可选的住院就诊记录主键，为空时查询该患者的全部住院记录。
	API_DTO_FIELD(String, encounterId,
		ZH_WORDS_GETTER("ovo.charging.settle.GetSettleHistoryQuery.encounterId.description"),
		false, "");

	// 结算状态筛选条件，ALL 表示包含结算和退结算记录。
	API_DTO_FIELD(String, statusEnum,
		ZH_WORDS_GETTER("ovo.charging.settle.GetSettleHistoryQuery.statusEnum.description"),
		false, "ALL");
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_SETTLE_HISTORY_QUERY_H_
