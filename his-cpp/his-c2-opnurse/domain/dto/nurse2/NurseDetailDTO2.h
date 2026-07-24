/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/07/24

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
#ifndef _NURSEDETAILDTO_H_
#define _NURSEDETAILDTO_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Nurse2 detail DTO (used in page query response)
class NurseDetailDTO2 : public oatpp::DTO
{
	DTO_INIT(NurseDetailDTO2, DTO);

	// Name / queue number
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("nurse2.field.name_code"), true);
	// Execution state
	API_DTO_FIELD_REQUIRE(String, state, ZH_WORDS_GETTER("nurse2.field.state"), false);
	// Medical order content
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("nurse2.field.content"), false);
	// Execution count
	API_DTO_FIELD_REQUIRE(Int32, amount, ZH_WORDS_GETTER("nurse2.field.amount"), false);
	// Executor
	API_DTO_FIELD_REQUIRE(String, person, ZH_WORDS_GETTER("nurse2.field.person"), false);
	// Operation type
	API_DTO_FIELD_REQUIRE(String, operate, ZH_WORDS_GETTER("nurse2.field.operate"), false);
};

// Nurse2 paged query response
class NurseDetailPageDTO : public PageDTO<NurseDetailDTO2::Wrapper>
{
	DTO_INIT(NurseDetailPageDTO, PageDTO<NurseDetailDTO2::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_NURSEDETAILDTO_H_
