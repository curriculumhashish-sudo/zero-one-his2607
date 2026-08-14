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
 * 住院患者队列查询对象，支持按病区和关键字筛选。
 */
class PatientQueueQuery : public PageQuery
{
	DTO_INIT(PatientQueueQuery, PageQuery);
	// 病区ID（adm_location 中 form_enum='WARD' 的ID）
	DTO_FIELD(String, wardId);
	DTO_FIELD_INFO(wardId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.wardId");
	}
	// 关键字，匹配住院号(bus_no)、患者姓名(name)或床号(location.name)
	DTO_FIELD(String, keyword);
	DTO_FIELD_INFO(keyword) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.keyword");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENT_QUEUE_QUERY_H_
