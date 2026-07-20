#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 10:59:38

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
#ifndef _DISCHARGE_ADVICE_DTO_H_
#define _DISCHARGE_ADVICE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 创建出院建议的请求DTO。
 * 需要就诊ID来标识患者。
 */
class CreateDischargeAdviceDTO : public oatpp::DTO
{
	DTO_INIT(CreateDischargeAdviceDTO, DTO);
	// Encounter (visit) ID
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("inpdocws.field.encounterId"), true);
	// 出院方式代码（1-医嘱离院, 2-转院, 3-转社区, 4-非医嘱离院, 5-死亡, 6-其他）
	DTO_FIELD(String, dischargeWay);
	DTO_FIELD_INFO(dischargeWay) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.dischargeWay");
	}
	// 出院日期时间
	DTO_FIELD(String, dischargeTime);
	DTO_FIELD_INFO(dischargeTime) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.dischargeTime");
	}
	// 出院诊断摘要
	DTO_FIELD(String, dischargeDiagnosis);
	DTO_FIELD_INFO(dischargeDiagnosis) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.dischargeDiagnosis");
	}

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	std::string validate() {
		if (!encounterId || encounterId->empty())
			return "encounterId is required.";
		return "";
	}
};

/**
 * 创建成功后返回的出院建议信息DTO。
 */
class DischargeAdviceDetailDTO : public oatpp::DTO
{
	DTO_INIT(DischargeAdviceDetailDTO, DTO);
	// 出院建议ID（服务请求ID）
	DTO_FIELD(String, adviceId);
	DTO_FIELD_INFO(adviceId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.adviceId");
	}
	// 就诊ID
	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.encounterId");
	}
	// 创建出院建议后的新状态（如：ON_HOLD待出院）
	DTO_FIELD(String, newStatus);
	DTO_FIELD_INFO(newStatus) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.newStatus");
	}
	// 状态显示文本
	DTO_FIELD(String, newStatusText);
	DTO_FIELD_INFO(newStatusText) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.newStatusText");
	}
	// 出院方式代码
	DTO_FIELD(String, dischargeWay);
	DTO_FIELD_INFO(dischargeWay) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.dischargeWay");
	}
	// 出院方式显示名称
	DTO_FIELD(String, dischargeWayText);
	DTO_FIELD_INFO(dischargeWayText) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.dischargeWayText");
	}
	// 出院时间
	DTO_FIELD(String, dischargeTime);
	DTO_FIELD_INFO(dischargeTime) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.dischargeTime");
	}
public:
	DischargeAdviceDetailDTO() {
		this->adviceId = "SR20260720001";
		this->encounterId = "337512373001654272";
		this->newStatus = "ON_HOLD";
		this->newStatusText = "Pending Discharge";
		this->dischargeWay = "1";
		this->dischargeWayText = "Medical Advice Discharge";
		this->dischargeTime = "2026-07-20 14:00:00";
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DISCHARGE_ADVICE_DTO_H_
