#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
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
#ifndef _MEDICATION_CATALOG_VO_H_
#define _MEDICATION_CATALOG_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/drug/MedicationCatalogDTO.h"
#include "domain/vo/JsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicationCatalogDetailVO : public MedicationCatalogModifyDTO
{
	DTO_INIT(MedicationCatalogDetailVO, MedicationCatalogModifyDTO);

	API_DTO_FIELD(String, medicationDefId, ZH_WORDS_GETTER("medication.field.medicationDefId"), true, "322994171216400384");
	API_DTO_FIELD(String, statusText, ZH_WORDS_GETTER("medication.field.statusText"), true, u8"\u542F\u7528");
	API_DTO_FIELD(Boolean, isDeleted, ZH_WORDS_GETTER("medication.field.isDeleted"), true, false);
	API_DTO_FIELD(Int32, version, ZH_WORDS_GETTER("medication.field.version"), true, 0);
	API_DTO_FIELD(String, createdAt, ZH_WORDS_GETTER("medication.field.createdAt"), true, "2026-07-14T10:30:00+08:00");
	API_DTO_FIELD(String, updatedAt, ZH_WORDS_GETTER("medication.field.updatedAt"), true, "2026-07-20T05:30:00+08:00");
	API_DTO_FIELD(String, createdBy, ZH_WORDS_GETTER("medication.field.createdBy"), false, "system");
	API_DTO_FIELD(String, updatedBy, ZH_WORDS_GETTER("medication.field.updatedBy"), false, "axiao");
};

class MedicationCatalogPageVO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogPageVO, DTO);

	API_DTO_FIELD(Int32, pageNo, ZH_WORDS_GETTER("medication.field.pageNo"), true, 1);
	API_DTO_FIELD(Int32, pageSize, ZH_WORDS_GETTER("medication.field.pageSize"), true, 10);
	API_DTO_FIELD(Int64, total, ZH_WORDS_GETTER("medication.field.total"), true, 15);
	API_DTO_FIELD(Int32, pages, ZH_WORDS_GETTER("medication.field.pages"), true, 2);
	DTO_FIELD(List<MedicationCatalogDetailVO::Wrapper>, rows) = List<MedicationCatalogDetailVO::Wrapper>::createShared();
	DTO_FIELD_INFO(rows) {
		info->description = ZH_WORDS_GETTER("medication.field.rows");
		info->required = true;
	}

public:
	MedicationCatalogPageVO()
	{
		rows->push_back(MedicationCatalogDetailVO::createShared());
	}
};

class MedicationCatalogPageJsonVO : public JsonVO<MedicationCatalogPageVO::Wrapper>
{
	DTO_INIT(MedicationCatalogPageJsonVO, JsonVO<MedicationCatalogPageVO::Wrapper>);

public:
	MedicationCatalogPageJsonVO()
	{
		this->success(MedicationCatalogPageVO::createShared());
	}
};

class MedicationCatalogDetailJsonVO : public JsonVO<MedicationCatalogDetailVO::Wrapper>
{
	DTO_INIT(MedicationCatalogDetailJsonVO, JsonVO<MedicationCatalogDetailVO::Wrapper>);

public:
	MedicationCatalogDetailJsonVO()
	{
		this->success(MedicationCatalogDetailVO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_CATALOG_VO_H_
