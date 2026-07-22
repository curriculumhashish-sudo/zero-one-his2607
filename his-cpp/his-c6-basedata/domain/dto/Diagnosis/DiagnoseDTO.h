#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei

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
#ifndef _DIAGNOSE_DTO_H_
#define _DIAGNOSE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Transfer object used to create a diagnosis record.
 */
class DiagnoseAddDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseAddDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("diagnose.field.diag.name"), true);
	API_DTO_FIELD_DEFAULT(String, diagType, ZH_WORDS_GETTER("diagnose.field.diag.diag_type"));
	API_DTO_FIELD_DEFAULT(String, chronicCode, ZH_WORDS_GETTER("diagnose.field.diag.chronic_code"));
	API_DTO_FIELD_DEFAULT(String, medicalInsuranceCode, ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_code"));
	API_DTO_FIELD_DEFAULT(Int32, status, ZH_WORDS_GETTER("diagnose.field.diag.status"));
	API_DTO_FIELD_DEFAULT(Int32, medicalInsuranceTag, ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_tag"));
	API_DTO_FIELD_DEFAULT(Int32, medicalInsuranceMatch, ZH_WORDS_GETTER("diagnose.field.diag.medical_insurance_match"));
	API_DTO_FIELD_DEFAULT(String, pinyinCode, ZH_WORDS_GETTER("diagnose.field.diag.pinyin_code"));
	API_DTO_FIELD_DEFAULT(String, wubiCode, ZH_WORDS_GETTER("diagnose.field.diag.wubi_code"));
	API_DTO_FIELD_DEFAULT(Int32, isDelete, ZH_WORDS_GETTER("diagnose.field.diag.is_delete"));
	API_DTO_FIELD_DEFAULT(String, remark, ZH_WORDS_GETTER("diagnose.field.diag.remark"));
	API_DTO_FIELD_REQUIRE(String, catName, ZH_WORDS_GETTER("diagnose.field.category.cat_name"), true);

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
	std::string validate()
	{
		if (!name || name->empty())
		{
			return "The disease name cannot be empty";
		}
		if (!catName || catName->empty())
		{
			return "The category name cannot be empty";
		}

		if (diagType && !diagType->empty()
			&& *diagType != "western_medicine_diagnosis"
			&& *diagType != "chinese_main_disease_diagnosis"
			&& *diagType != "chinese_main_symptom_diagnosis")
		{
			return "The diagnosis type is invalid";
		}

		if (status && *status != 0 && *status != 1)
		{
			return "The status only allows 0 or 1";
		}
		if (medicalInsuranceTag && *medicalInsuranceTag != 0 && *medicalInsuranceTag != 1)
		{
			return "The medical insurance flag only allows 0 or 1";
		}
		if (medicalInsuranceMatch && *medicalInsuranceMatch != 0 && *medicalInsuranceMatch != 1)
		{
			return "The medical insurance match flag only allows 0 or 1";
		}
		if (isDelete && *isDelete != 0 && *isDelete != 1)
		{
			return "The delete flag only allows 0 or 1";
		}

		return "";
	}
};

/**
 * Transfer object used to update a diagnosis record.
 */
class DiagnoseDTO : public DiagnoseAddDTO
{
	DTO_INIT(DiagnoseDTO, DiagnoseAddDTO);

	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("diagnose.field.diag.id"), true);
};

/**
 * Transfer object used to change diagnosis statuses in a batch.
 */
class DiagnoseBatchStatusDTO : public oatpp::DTO
{
	DTO_INIT(DiagnoseBatchStatusDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, ids, ZH_WORDS_GETTER("diagnose.field.diag.id_list"), true);
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("diagnose.field.diag.status"), true);

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSE_DTO_H_
