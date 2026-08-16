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
#ifndef _PATIENT_QUEUE_DTO_H_
#define _PATIENT_QUEUE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 住院患者队列中的单个患者数据传输对象。
 * 包含患者基本信息、诊断摘要、床位信息和账户余额。
 */
class PatientQueueDTO : public oatpp::DTO
{
	DTO_INIT(PatientQueueDTO, DTO);
	// 就诊ID（就诊记录主键）
	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.encounterId");
	}
	// 患者ID
	DTO_FIELD(String, patientId);
	DTO_FIELD_INFO(patientId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.patientId");
	}
	// 患者姓名
	DTO_FIELD(String, patientName);
	DTO_FIELD_INFO(patientName) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.patientName");
	}
	// 性别
	DTO_FIELD(String, gender);
	DTO_FIELD_INFO(gender) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.gender");
	}
	// 年龄
	DTO_FIELD(Int32, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.age");
	}
	// 诊断摘要
	DTO_FIELD(String, diagnosisSummary);
	DTO_FIELD_INFO(diagnosisSummary) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.diagnosisSummary");
	}
	// 床位名称（床号）
	DTO_FIELD(String, bedName);
	DTO_FIELD_INFO(bedName) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.bedName");
	}
	// 床位位置ID
	DTO_FIELD(String, bedId);
	DTO_FIELD_INFO(bedId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.bedId");
	}
	// 病区ID
	DTO_FIELD(String, wardId);
	DTO_FIELD_INFO(wardId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.wardId");
	}
	// 病区名称
	DTO_FIELD(String, wardName);
	DTO_FIELD_INFO(wardName) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.wardName");
	}
	// 病房号
	DTO_FIELD(String, houseName);
	DTO_FIELD_INFO(houseName) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.houseName");
	}
	// 账户余额
	DTO_FIELD(Float64, accountBalance);
	DTO_FIELD_INFO(accountBalance) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.accountBalance");
	}
	// 就诊状态（adm_encounter.status_enum）
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.statusEnum");
	}
	// 住院号（adm_encounter.bus_no）
	DTO_FIELD(String, encounterBusNo);
	DTO_FIELD_INFO(encounterBusNo) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.encounterBusNo");
	}
	// 入院时间（adm_encounter.start_time）
	DTO_FIELD(String, admitTime);
	DTO_FIELD_INFO(admitTime) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.admitTime");
	}
	// 科室ID（adm_encounter.organization_id）
	DTO_FIELD(String, organizationId);
	DTO_FIELD_INFO(organizationId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.organizationId");
	}
	// 科室名称（adm_organization.name）
	DTO_FIELD(String, organizationName);
	DTO_FIELD_INFO(organizationName) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.organizationName");
	}
public:
	PatientQueueDTO() {}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PATIENT_QUEUE_DTO_H_
