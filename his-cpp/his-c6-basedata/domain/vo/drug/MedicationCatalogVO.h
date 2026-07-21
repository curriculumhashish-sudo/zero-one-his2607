#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: bandao
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

class MedicationCatalogUnitBandVO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogUnitBandVO, DTO);

	API_DTO_FIELD(String, bandKind, ZH_WORDS_GETTER("medication.field.bandKind"), true, "PACKAGE");
	API_DTO_FIELD(String, unitCode, ZH_WORDS_GETTER("medication.field.unitCode"), true, "DW003");
	API_DTO_FIELD(String, unitName, ZH_WORDS_GETTER("medication.field.unitName"), true,
		ZH_WORDS_GETTER("medication.example.purchaseUnitText"));
	API_DTO_FIELD(Float64, ratioToSplit, ZH_WORDS_GETTER("medication.field.ratioToSplit"), true, 10.0);
	API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("medication.field.unitPrice"), true, 15.8);
};

class MedicationCatalogDetailVO : public MedicationCatalogBaseDTO
{
	DTO_INIT(MedicationCatalogDetailVO, MedicationCatalogBaseDTO);

	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("medication.field.id"), true);
	API_DTO_FIELD(String, specification, ZH_WORDS_GETTER("medication.field.specification"), true, "5ml:0.5g");
	API_DTO_FIELD(String, activeFlagText, ZH_WORDS_GETTER("medication.field.activeFlagText"), true,
		ZH_WORDS_GETTER("medication.example.statusEnabled"));
	API_DTO_FIELD(String, categoryCode_dictText,
		ZH_WORDS_GETTER("medication.field.categoryCodeText"), true,
		ZH_WORDS_GETTER("medication.example.typeText"));
	API_DTO_FIELD(String, packageUnitCode_dictText,
		ZH_WORDS_GETTER("medication.field.packageUnitCodeText"), true,
		ZH_WORDS_GETTER("medication.example.purchaseUnitText"));
	API_DTO_FIELD(String, locationId_dictText,
		ZH_WORDS_GETTER("medication.field.locationIdText"), false,
		ZH_WORDS_GETTER("medication.example.locationText"));
	API_DTO_FIELD(String, supplyId_dictText,
		ZH_WORDS_GETTER("medication.field.supplyIdText"), true,
		ZH_WORDS_GETTER("medication.example.supplierText"));
	API_DTO_FIELD(Boolean, potencyFlag, ZH_WORDS_GETTER("medication.field.potencyFlag"), false, false);
	API_DTO_FIELD(List<MedicationCatalogUnitBandVO::Wrapper>, unitBands,
		ZH_WORDS_GETTER("medication.field.unitBands"), true,
		List<MedicationCatalogUnitBandVO::Wrapper>::createShared());

public:
	MedicationCatalogDetailVO()
	{
		unitBands->push_back(MedicationCatalogUnitBandVO::createShared());
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
