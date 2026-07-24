#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/24 00:00:00

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
#ifndef _TREATMENTREMOVEDTO_H_
#define _TREATMENTREMOVEDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Batch remove treatment DTO
 */
class TreatmentRemoveDTO : public oatpp::DTO
{
	DTO_INIT(TreatmentRemoveDTO, DTO);
	// Primary key list of treatment records to remove
	API_DTO_FIELD_REQUIRE(List<String>, ids, ZH_WORDS_GETTER("treatment.field.ids"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTREMOVEDTO_H_
