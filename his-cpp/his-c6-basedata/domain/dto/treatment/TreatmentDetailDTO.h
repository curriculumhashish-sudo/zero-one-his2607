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
#ifndef _TREATMENTDETAILDTO_H_
#define _TREATMENTDETAILDTO_H_

#include "../../GlobalInclude.h"
#include "TreatmentChildDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class TreatmentDetailDTO : public oatpp::DTO
{
	DTO_INIT(TreatmentDetailDTO, DTO);

	// 诊疗项目主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("treatment.field.id"), true);
	// 诊疗项目业务编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("treatment.field.code"), true);
	// 启用/禁用状态
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("treatment.field.status"), true);

	// 基本信息
	// 诊疗项目名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("treatment.field.name"), true);
	// 类别编码
	API_DTO_FIELD_REQUIRE(String, categoryCode, ZH_WORDS_GETTER("treatment.field.category-code"), true);
	// 业务类型枚举
	API_DTO_FIELD_REQUIRE(Int32, typeEnum, ZH_WORDS_GETTER("treatment.field.type-enum"), true);
	// 销售/定价单位编码
	API_DTO_FIELD_REQUIRE(String, packageUnitCode, ZH_WORDS_GETTER("treatment.field.package-unit-code"), false);
	// 拼音快速码
	API_DTO_FIELD_REQUIRE(String, pyStr, ZH_WORDS_GETTER("treatment.field.py-str"), false);
	// 五笔快速码
	API_DTO_FIELD_REQUIRE(String, wbStr, ZH_WORDS_GETTER("treatment.field.wb-str"), false);
	// 财务类型编码
	API_DTO_FIELD_REQUIRE(String, itemTypeCode, ZH_WORDS_GETTER("treatment.field.item-type-code"), false);
	// 采购/成本价
	API_DTO_FIELD_REQUIRE(Float64, purchasePrice, ZH_WORDS_GETTER("treatment.field.purchase-price"), false);
	// 零售价
	API_DTO_FIELD_REQUIRE(Float64, retailPrice, ZH_WORDS_GETTER("treatment.field.retail-price"), false);
	// 最高零售价
	API_DTO_FIELD_REQUIRE(Float64, maximumRetailPrice, ZH_WORDS_GETTER("treatment.field.maximum-retail-price"), false);
	// 描述文本
	API_DTO_FIELD_REQUIRE(String, descriptionText, ZH_WORDS_GETTER("treatment.field.description-text"), false);

	// 医保信息
	// 医保标志
	API_DTO_FIELD_REQUIRE(Int32, ybFlag, ZH_WORDS_GETTER("treatment.field.yb-flag"), false);
	// 医保登记编码
	API_DTO_FIELD_REQUIRE(String, ybNo, ZH_WORDS_GETTER("treatment.field.yb-no"), false);
	// 医保类别编码
	API_DTO_FIELD_REQUIRE(String, ybType, ZH_WORDS_GETTER("treatment.field.yb-type"), false);
	// 医保对码标志
	API_DTO_FIELD_REQUIRE(Int32, ybMatchFlag, ZH_WORDS_GETTER("treatment.field.yb-match-flag"), false);
	// 医保收费项目等级
	API_DTO_FIELD_REQUIRE(Int32, chrgitmLv, ZH_WORDS_GETTER("treatment.field.chrgitm-lv"), false);

	// 子项
	API_DTO_FIELD_REQUIRE(List<TreatmentChildDTO::Wrapper>, children, ZH_WORDS_GETTER("treatment.field.children"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif 
