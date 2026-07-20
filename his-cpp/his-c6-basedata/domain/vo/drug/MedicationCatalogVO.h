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
	API_DTO_FIELD(String, unitCode, ZH_WORDS_GETTER("medication.field.unitCode"), true, "2");
	API_DTO_FIELD(String, unitName, ZH_WORDS_GETTER("medication.field.unitName"), true, u8"\u76D2");
	API_DTO_FIELD(Float64, ratioToSplit, ZH_WORDS_GETTER("medication.field.ratioToSplit"), true, 20.0);
	API_DTO_FIELD(Float64, unitPrice, ZH_WORDS_GETTER("medication.field.unitPrice"), true, 6.99);
};

class MedicationCatalogListItemVO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogListItemVO, DTO);

	API_DTO_FIELD(String, id, ZH_WORDS_GETTER("medication.field.id"), true, "322994171216400384");
	API_DTO_FIELD(String, medicationDefId, ZH_WORDS_GETTER("medication.field.medicationDefId"), true, "322994171216400384");
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("medication.field.statusEnum"), true, "ACTIVE");
	API_DTO_FIELD(String, statusEnum_enumText, ZH_WORDS_GETTER("medication.field.statusEnumText"), true, u8"\u5DF2\u542F\u7528");
	API_DTO_FIELD(Boolean, activeFlag, ZH_WORDS_GETTER("medication.field.activeFlag"), true, true);
	API_DTO_FIELD(String, activeFlagText, ZH_WORDS_GETTER("medication.field.activeFlagText"), true, u8"\u542F\u7528");
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("medication.field.busNo"), true, "MD0021");
	API_DTO_FIELD(String, name, ZH_WORDS_GETTER("medication.field.name"), true, u8"\u5934\u5B62\u514B\u809F\u80F6\u56CA");
	API_DTO_FIELD(String, specification, ZH_WORDS_GETTER("medication.field.specification"), true, "0.1g");
	API_DTO_FIELD(String, categoryCode, ZH_WORDS_GETTER("medication.field.categoryCode"), true, "2");
	API_DTO_FIELD(String, categoryCode_dictText, ZH_WORDS_GETTER("medication.field.categoryCodeText"), true, u8"\u897F\u836F");
	API_DTO_FIELD(String, manufacturerId, ZH_WORDS_GETTER("medication.field.manufacturerId"), false, "4100000000000000001");
	API_DTO_FIELD(String, manufacturerText, ZH_WORDS_GETTER("medication.field.manufacturerText"), true, u8"\u534E\u5317\u5236\u836F\u6CB3\u5317\u534E\u6C11\u836F\u4E1A\u6709\u9650\u8D23\u4EFB\u516C\u53F8");
	API_DTO_FIELD(String, supplyId, ZH_WORDS_GETTER("medication.field.supplyId"), true, "4100000000000000004");
	API_DTO_FIELD(String, supplyId_dictText, ZH_WORDS_GETTER("medication.field.supplyIdText"), true, u8"\u6CB3\u5317\u533B\u836F\u4F9B\u5E94\u5546");
	API_DTO_FIELD(String, ybNo, ZH_WORDS_GETTER("medication.field.ybNo"), false, "YA01BAA0001001010101");
	API_DTO_FIELD(Boolean, ybMatchFlag, ZH_WORDS_GETTER("medication.field.ybMatchFlag"), false, true);
	API_DTO_FIELD(String, antibioticCode, ZH_WORDS_GETTER("medication.field.antibioticCode"), false, "J01DD");
	API_DTO_FIELD(String, antibioticCode_dictText, ZH_WORDS_GETTER("medication.field.antibioticCodeText"), false, u8"\u5934\u5B62\u83CC\u7D20\u7C7B");
	API_DTO_FIELD(String, doseUnitCode, ZH_WORDS_GETTER("medication.field.doseUnitCode"), true, "10");
	API_DTO_FIELD(Float64, doseUnitRatio, ZH_WORDS_GETTER("medication.field.doseUnitRatio"), true, 0.1);
	API_DTO_FIELD(String, purchaseUnitCode, ZH_WORDS_GETTER("medication.field.purchaseUnitCode"), true, "2");
	API_DTO_FIELD(Float64, purchaseUnitRatio, ZH_WORDS_GETTER("medication.field.purchaseUnitRatio"), true, 20.0);
	API_DTO_FIELD(Float64, purchasePrice, ZH_WORDS_GETTER("medication.field.purchasePrice"), true, 6.99);
	API_DTO_FIELD(String, splitUnitCode, ZH_WORDS_GETTER("medication.field.splitUnitCode"), true, "5");
	API_DTO_FIELD(String, packageUnitCode, ZH_WORDS_GETTER("medication.field.packageUnitCode"), true, "2");
	API_DTO_FIELD(String, packageUnitCode_dictText, ZH_WORDS_GETTER("medication.field.packageUnitCodeText"), true, u8"\u76D2");
	API_DTO_FIELD(Float64, packageUnitRatio, ZH_WORDS_GETTER("medication.field.packageUnitRatio"), true, 20.0);
	API_DTO_FIELD(String, stockUnitCode, ZH_WORDS_GETTER("medication.field.stockUnitCode"), true, "2");
	API_DTO_FIELD(Float64, stockUnitRatio, ZH_WORDS_GETTER("medication.field.stockUnitRatio"), true, 20.0);
	API_DTO_FIELD(String, locationId, ZH_WORDS_GETTER("medication.field.locationId"), false, "322663105141149697");
	API_DTO_FIELD(String, locationId_dictText, ZH_WORDS_GETTER("medication.field.locationIdText"), false, u8"\u95E8\u8BCA\u836F\u623F");
	API_DTO_FIELD(Float64, retailPrice, ZH_WORDS_GETTER("medication.field.retailPrice"), true, 6.99);
	API_DTO_FIELD(Float64, maxUnit, ZH_WORDS_GETTER("medication.field.maxUnit"), false, 2.0);
	API_DTO_FIELD(Boolean, rxFlag, ZH_WORDS_GETTER("medication.field.rxFlag"), false, false);
	API_DTO_FIELD(String, version, ZH_WORDS_GETTER("medication.field.version"), true, "1");
	DTO_FIELD(List<MedicationCatalogUnitBandVO::Wrapper>, unitBands) =
		List<MedicationCatalogUnitBandVO::Wrapper>::createShared();
	DTO_FIELD_INFO(unitBands)
	{
		info->description = ZH_WORDS_GETTER("medication.field.unitBands");
		info->required = true;
	}

public:
	MedicationCatalogListItemVO()
	{
		unitBands->push_back(MedicationCatalogUnitBandVO::createShared());
	}
};

class MedicationCatalogDetailVO : public MedicationCatalogModifyDTO
{
	DTO_INIT(MedicationCatalogDetailVO, MedicationCatalogModifyDTO);

	API_DTO_FIELD(String, medicationDefId, ZH_WORDS_GETTER("medication.field.medicationDefId"), true, "322994171216400384");
	API_DTO_FIELD(String, specification, ZH_WORDS_GETTER("medication.field.specification"), true, "0.1g");
	API_DTO_FIELD(String, statusEnum_enumText, ZH_WORDS_GETTER("medication.field.statusEnumText"), true, u8"\u5DF2\u542F\u7528");
	API_DTO_FIELD(String, activeFlagText, ZH_WORDS_GETTER("medication.field.activeFlagText"), true, u8"\u542F\u7528");
	API_DTO_FIELD(String, categoryCode_dictText, ZH_WORDS_GETTER("medication.field.categoryCodeText"), true, u8"\u897F\u836F");
	API_DTO_FIELD(String, doseFrom_dictText, ZH_WORDS_GETTER("medication.field.doseFromText"), false, u8"\u56FA\u4F53");
	API_DTO_FIELD(String, doseUnitCode_dictText, ZH_WORDS_GETTER("medication.field.doseUnitCodeText"), true, "g");
	API_DTO_FIELD(String, purchaseUnitCode_dictText, ZH_WORDS_GETTER("medication.field.purchaseUnitCodeText"), true, u8"\u76D2");
	API_DTO_FIELD(String, packageUnitCode_dictText, ZH_WORDS_GETTER("medication.field.packageUnitCodeText"), true, u8"\u76D2");
	API_DTO_FIELD(String, locationId_dictText, ZH_WORDS_GETTER("medication.field.locationIdText"), false, u8"\u95E8\u8BCA\u836F\u623F");
	API_DTO_FIELD(String, supplyId_dictText, ZH_WORDS_GETTER("medication.field.supplyIdText"), true, u8"\u6CB3\u5317\u533B\u836F\u4F9B\u5E94\u5546");
	API_DTO_FIELD(String, typeCode_dictText, ZH_WORDS_GETTER("medication.field.typeCodeText"), true, u8"\u897F\u836F");
	API_DTO_FIELD(String, antibioticCode_dictText, ZH_WORDS_GETTER("medication.field.antibioticCodeText"), false, u8"\u5934\u5B62\u83CC\u7D20\u7C7B");
	API_DTO_FIELD(Boolean, potencyFlag, ZH_WORDS_GETTER("medication.field.potencyFlag"), false, false);
	API_DTO_FIELD(String, unitBandsJson, ZH_WORDS_GETTER("medication.field.unitBandsJson"), false, "[]");
	DTO_FIELD(List<MedicationCatalogUnitBandVO::Wrapper>, unitBands) =
		List<MedicationCatalogUnitBandVO::Wrapper>::createShared();
	DTO_FIELD_INFO(unitBands)
	{
		info->description = ZH_WORDS_GETTER("medication.field.unitBands");
		info->required = true;
	}
	API_DTO_FIELD(Boolean, isDeleted, ZH_WORDS_GETTER("medication.field.isDeleted"), false, false);
	API_DTO_FIELD(String, createdAt, ZH_WORDS_GETTER("medication.field.createdAt"), false, "2026-07-14T10:30:00+08:00");
	API_DTO_FIELD(String, updatedAt, ZH_WORDS_GETTER("medication.field.updatedAt"), false, "2026-07-20T05:30:00+08:00");
	API_DTO_FIELD(String, createdBy, ZH_WORDS_GETTER("medication.field.createdBy"), false, "system");
	API_DTO_FIELD(String, updatedBy, ZH_WORDS_GETTER("medication.field.updatedBy"), false, "bandao");

public:
	MedicationCatalogDetailVO()
	{
		unitBands->push_back(MedicationCatalogUnitBandVO::createShared());
	}
};

class MedicationCatalogPageVO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogPageVO, DTO);

	API_DTO_FIELD(Int32, pageNo, ZH_WORDS_GETTER("medication.field.pageNo"), true, 1);
	API_DTO_FIELD(Int32, pageSize, ZH_WORDS_GETTER("medication.field.pageSize"), true, 50);
	API_DTO_FIELD(Int64, totalElements, ZH_WORDS_GETTER("medication.field.totalElements"), true, 14);
	API_DTO_FIELD(Int64, totalCount, ZH_WORDS_GETTER("medication.field.totalCount"), true, 14);
	API_DTO_FIELD(Int32, totalPages, ZH_WORDS_GETTER("medication.field.totalPages"), true, 1);
	DTO_FIELD(List<MedicationCatalogListItemVO::Wrapper>, content) =
		List<MedicationCatalogListItemVO::Wrapper>::createShared();
	DTO_FIELD_INFO(content)
	{
		info->description = ZH_WORDS_GETTER("medication.field.content");
		info->required = true;
	}

public:
	MedicationCatalogPageVO()
	{
		content->push_back(MedicationCatalogListItemVO::createShared());
	}
};

class MedicationCatalogOptionVO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogOptionVO, DTO);

	API_DTO_FIELD(String, value, ZH_WORDS_GETTER("medication.field.optionValue"), true, "ACTIVE");
	API_DTO_FIELD(String, label, ZH_WORDS_GETTER("medication.field.optionLabel"), true, u8"\u542F\u7528");
	API_DTO_FIELD(String, sort, ZH_WORDS_GETTER("medication.field.optionSort"), false, "0");
	API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("medication.field.optionBusNo"), false, "");
};

class MedicationCatalogInitVO : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogInitVO, DTO);

	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, statusFlagOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, domainFlagOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, supplierListOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, medicationCategoryCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, unitCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, chrgitmLvOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, doseFormCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, methodCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, rateCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, dosageInstructionOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, finTypeCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, medicinePropertiesOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, medChrgitmTypeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, rxFlagOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, partAttributeEnumOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, tempOrderSplitPropertyEnumOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, locationOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, antibioticTypeCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, dddCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, doseFromCodeOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, statusRestrictedOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, statusWeatherOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();
	DTO_FIELD(List<MedicationCatalogOptionVO::Wrapper>, systemCategoriesOptions) = List<MedicationCatalogOptionVO::Wrapper>::createShared();

public:
	MedicationCatalogInitVO()
	{
		statusFlagOptions->push_back(createOption("ACTIVE", u8"\u542F\u7528"));
		domainFlagOptions->push_back(createOption("WESTERN_MEDICINE", u8"\u897F\u836F"));
		supplierListOptions->push_back(createOption("4100000000000000004", u8"\u6CB3\u5317\u533B\u836F\u4F9B\u5E94\u5546", "SUP0004"));
		medicationCategoryCodeOptions->push_back(createOption("2", u8"\u897F\u836F"));
		unitCodeOptions->push_back(createOption("2", u8"\u76D2"));
		chrgitmLvOptions->push_back(createOption("1", u8"\u7532\u7C7B"));
		doseFormCodeOptions->push_back(createOption("5", u8"\u80F6\u56CA\u5242"));
		methodCodeOptions->push_back(createOption("1", u8"\u53E3\u670D"));
		rateCodeOptions->push_back(createOption("BID", u8"\u6BCF\u65E5\u4E24\u6B21"));
		dosageInstructionOptions->push_back(createOption("1", u8"\u5148\u714E"));
		finTypeCodeOptions->push_back(createOption("1001", u8"\u897F\u836F"));
		medicinePropertiesOptions->push_back(createOption("1", u8"\u666E\u901A\u836F\u54C1"));
		medChrgitmTypeOptions->push_back(createOption("1", u8"\u897F\u836F"));
		rxFlagOptions->push_back(createOption("false", u8"\u975E\u5904\u65B9\u836F"));
		partAttributeEnumOptions->push_back(createOption("2", u8"\u53EF\u62C6\u5206"));
		tempOrderSplitPropertyEnumOptions->push_back(createOption("2", u8"\u53EF\u62C6\u5206"));
		locationOptions->push_back(createOption("322663105141149697", u8"\u95E8\u8BCA\u836F\u623F"));
		antibioticTypeCodeOptions->push_back(createOption("J01DD", u8"\u5934\u5B62\u83CC\u7D20\u7C7B"));
		dddCodeOptions->push_back(createOption("J01DD08", "J01DD08"));
		doseFromCodeOptions->push_back(createOption("1", u8"\u56FA\u4F53"));
		statusRestrictedOptions->push_back(createOption("NONE", u8"\u4E0D\u9650\u5236"));
		statusWeatherOptions->push_back(createOption("NORMAL", u8"\u5E38\u6E29"));
		systemCategoriesOptions->push_back(createOption("MEDICATION", u8"\u836F\u54C1"));
	}

private:
	static MedicationCatalogOptionVO::Wrapper createOption(const String& value, const String& label,
		const String& busNo = "")
	{
		auto option = MedicationCatalogOptionVO::createShared();
		option->value = value;
		option->label = label;
		option->busNo = busNo;
		return option;
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

class MedicationCatalogInitJsonVO : public JsonVO<MedicationCatalogInitVO::Wrapper>
{
	DTO_INIT(MedicationCatalogInitJsonVO, JsonVO<MedicationCatalogInitVO::Wrapper>);

public:
	MedicationCatalogInitJsonVO()
	{
		this->success(MedicationCatalogInitVO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_CATALOG_VO_H_
