#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:36:29

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
#ifndef _PATIENT_QUEUE_QUERY_H_
#define _PATIENT_QUEUE_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者队列查询对象，用于筛选在院患者列表。
 * 支持按病区、关键字（住院号/姓名/床号）搜索及分页。
 */
class PatientQueueQuery : public PageQuery
{
	DTO_INIT(PatientQueueQuery, PageQuery);
	// 病区ID，用于按病区筛选患者
	DTO_FIELD(String, wardId);
	DTO_FIELD_INFO(wardId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.wardId");
	}
	// 关键字，支持按住院号、患者姓名或床号搜索
	DTO_FIELD(String, keyword);
	DTO_FIELD_INFO(keyword) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.keyword");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENT_QUEUE_QUERY_H_
