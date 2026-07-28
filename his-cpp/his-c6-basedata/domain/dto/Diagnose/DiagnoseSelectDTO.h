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
#ifndef _DIAGNOSIS_SELECT_DTO_H_
#define _DIAGNOSIS_SELECT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

<<<<<<< HEAD:his-cpp/his-c6-basedata/domain/dto/Diagnose/DiagnoseSelectDTO.h
/**
 * One selectable diagnose item returned by the diagnose picker.
 */
class DiagnoseSelectDTO : public oatpp::DTO
=======
/** 可选诊疗列表项。 */
class DiagnosisSelectDTO : public oatpp::DTO
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b:his-cpp/his-c6-basedata/domain/dto/Diagnosis/DiagnosisSelectDTO.h
{
	DTO_INIT(DiagnoseSelectDTO, DTO);

	API_DTO_FIELD(String, conditionDefinitionId,
		ZH_WORDS_GETTER("diagnose.field.select.condition_definition_id"), true, "317932056501817344");
	API_DTO_FIELD(String, conditionCode,
		ZH_WORDS_GETTER("diagnose.field.select.condition_code"), true, "CDF00000003");
	API_DTO_FIELD(String, conditionDefinitionName,
		ZH_WORDS_GETTER("diagnose.field.select.condition_definition_name"), true, u8"\u611f\u5192");
	API_DTO_FIELD(String, pyStr, ZH_WORDS_GETTER("diagnose.field.select.py_str"), false, "GM");
	API_DTO_FIELD(String, wbStr, ZH_WORDS_GETTER("diagnose.field.select.wb_str"), false, "DJ");
<<<<<<< HEAD:his-cpp/his-c6-basedata/domain/dto/Diagnose/DiagnoseSelectDTO.h
	API_DTO_FIELD(String, diagnoseKind, ZH_WORDS_GETTER("diagnose.field.select.diagnose_kind"), true, "WESTERN");
	API_DTO_FIELD(String, diagnoseKindText, ZH_WORDS_GETTER("diagnose.field.select.diagnose_kind_text"), true, u8"\u897f\u533b");
	API_DTO_FIELD(Int32, sourceEnum, ZH_WORDS_GETTER("diagnose.field.select.source_enum"), false, 1);
	API_DTO_FIELD(String, sourceEnumText, ZH_WORDS_GETTER("diagnose.field.select.source_enum_text"), true, u8"\u75be\u75c5\u4e0e\u8bca\u65ad");
=======
	API_DTO_FIELD(String, diagnosisKind,
		ZH_WORDS_GETTER("diagnose.field.select.diagnosis_kind"), true, "WESTERN");
	API_DTO_FIELD(String, diagnosisKindText,
		ZH_WORDS_GETTER("diagnose.field.select.diagnosis_kind_text"), true, u8"\u897f\u533b");
	API_DTO_FIELD(Int32, sourceEnum,
		ZH_WORDS_GETTER("diagnose.field.select.source_enum"), false, 1);
	API_DTO_FIELD(String, sourceEnumText,
		ZH_WORDS_GETTER("diagnose.field.select.source_enum_text"), false, u8"\u666e\u901a\u8bca\u65ad");
>>>>>>> 7a964f5c8d17ed83b384e758fad9c7e6fabfd56b:his-cpp/his-c6-basedata/domain/dto/Diagnosis/DiagnosisSelectDTO.h
	API_DTO_FIELD(Int32, ybFlag, ZH_WORDS_GETTER("diagnose.field.select.yb_flag"), true, 0);
	API_DTO_FIELD(String, ybFlagText,
		ZH_WORDS_GETTER("diagnose.field.select.yb_flag_text"), true, u8"\u5426");
	API_DTO_FIELD(Int32, statusEnum, ZH_WORDS_GETTER("diagnose.field.select.status_enum"), true, 2);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSIS_SELECT_DTO_H_
