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
#ifndef _TREATMENTCHILDDTO_H_
#define _TREATMENTCHILDDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Treatment sub-item DTO (a single row inside the "children" list of a treatment detail).
 */
class TreatmentChildDTO : public oatpp::DTO
{
	DTO_INIT(TreatmentChildDTO, DTO);
	// Sub-item row primary key
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.child-id"), false);
	// Referenced treatment item id selected as the sub-item
	API_DTO_FIELD_REQUIRE(String, childrenId, ZH_WORDS_GETTER("treatment.field.children-id"), true);
	// Display name of the referenced treatment item
	API_DTO_FIELD_REQUIRE(String, childrenName, ZH_WORDS_GETTER("treatment.field.children-name"), true);
	// Execution count for the sub-item
	API_DTO_FIELD_REQUIRE(Int32, childrenNum, ZH_WORDS_GETTER("treatment.field.children-num"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTCHILDDTO_H_
