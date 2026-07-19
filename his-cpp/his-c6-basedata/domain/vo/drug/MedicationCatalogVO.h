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

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicationCatalogPageDTO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogPageDTO, DTO);

	API_DTO_FIELD(Int32, pageNo, ZH_WORDS_GETTER("medication.field.pageNo"), true, 1);
	API_DTO_FIELD(Int32, pageSize, ZH_WORDS_GETTER("medication.field.pageSize"), true, 10);
	API_DTO_FIELD(Int64, total, ZH_WORDS_GETTER("medication.field.total"), true, 15);
	API_DTO_FIELD(Int32, pages, ZH_WORDS_GETTER("medication.field.pages"), true, 2);
	DTO_FIELD(List<MedicationCatalogDetailDTO::Wrapper>, rows) = List<MedicationCatalogDetailDTO::Wrapper>::createShared();
	DTO_FIELD_INFO(rows) {
		info->description = ZH_WORDS_GETTER("medication.field.rows");
		info->required = true;
	}

public:
	MedicationCatalogPageDTO()
	{
		rows->push_back(MedicationCatalogDetailDTO::createShared());
	}
};

class MedicationCatalogPageJsonVO : public JsonVO<MedicationCatalogPageDTO::Wrapper>
{
	DTO_INIT(MedicationCatalogPageJsonVO, JsonVO<MedicationCatalogPageDTO::Wrapper>);

public:
	MedicationCatalogPageJsonVO()
	{
		this->success(MedicationCatalogPageDTO::createShared());
	}
};

class MedicationCatalogDetailJsonVO : public JsonVO<MedicationCatalogDetailDTO::Wrapper>
{
	DTO_INIT(MedicationCatalogDetailJsonVO, JsonVO<MedicationCatalogDetailDTO::Wrapper>);

public:
	MedicationCatalogDetailJsonVO()
	{
		this->success(MedicationCatalogDetailDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_CATALOG_VO_H_
