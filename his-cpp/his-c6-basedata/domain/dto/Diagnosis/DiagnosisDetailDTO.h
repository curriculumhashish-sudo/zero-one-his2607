#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:00:34

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
#ifndef _DIAGNOSIS_DETAIL_DTO_H_
#define _DIAGNOSIS_DETAIL_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Diagnosis catalog detail used by both query-one and query-all.
 */
class DiagnosisDetailDTO : public oatpp::DTO
{
	DTO_INIT(DiagnosisDetailDTO, DTO);

	API_DTO_FIELD(Int64, id, ZH_WORDS_GETTER("diagnose.field.catalog.id"), true, 317932056501817344LL);
	API_DTO_FIELD(String, sourceEnum, ZH_WORDS_GETTER("diagnose.field.catalog.source_enum"), true, "general_dx");

	DTO_FIELD(String, sourceEnumText, "sourceEnum_enumText") = u8"\u75be\u75c5\u4e0e\u8bca\u65ad";
	DTO_FIELD_INFO(sourceEnumText) {
		info->description = ZH_WORDS_GETTER("diagnose.field.catalog.source_enum_text");
		info->required = true;
	}

	API_DTO_FIELD(String, conditionCode, ZH_WORDS_GETTER("diagnose.field.catalog.condition_code"), true, "CDF00000003");
	API_DTO_FIELD(String, diseaseCode, ZH_WORDS_GETTER("diagnose.field.catalog.disease_code"), false, "J00");
	API_DTO_FIELD(String, name, ZH_WORDS_GETTER("diagnose.field.catalog.name"), true, u8"\u611f\u5192");
	API_DTO_FIELD(String, typeCode, ZH_WORDS_GETTER("diagnose.field.catalog.type_code"), false, "1");

	DTO_FIELD(String, typeCodeText, "typeCode_dictText") = u8"\u897f\u533b\u8bca\u65ad";
	DTO_FIELD_INFO(typeCodeText) {
		info->description = ZH_WORDS_GETTER("diagnose.field.catalog.type_code_text");
		info->required = false;
	}

	API_DTO_FIELD(String, description, ZH_WORDS_GETTER("diagnose.field.catalog.description"), false, "Common cold");
	API_DTO_FIELD(String, pyStr, ZH_WORDS_GETTER("diagnose.field.catalog.py_str"), false, "GM");
	API_DTO_FIELD(String, wbStr, ZH_WORDS_GETTER("diagnose.field.catalog.wb_str"), false, "DJ");
	API_DTO_FIELD(Int32, ybFlag, ZH_WORDS_GETTER("diagnose.field.catalog.yb_flag"), false, 0);
	API_DTO_FIELD(String, ybNo, ZH_WORDS_GETTER("diagnose.field.catalog.yb_no"), false, "1.002");
	API_DTO_FIELD(Int32, ybMatchFlag, ZH_WORDS_GETTER("diagnose.field.catalog.yb_match_flag"), false, 0);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("diagnose.field.catalog.status_enum"), true, "ACTIVE");

	DTO_FIELD(String, statusEnumText, "statusEnum_enumText") = u8"\u5df2\u542f\u7528";
	DTO_FIELD_INFO(statusEnumText) {
		info->description = ZH_WORDS_GETTER("diagnose.field.catalog.status_enum_text");
		info->required = true;
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSIS_DETAIL_DTO_H_
