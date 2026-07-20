/*
 Copyright Zero One Star. All rights reserved.

 撤回中药处方请求 DTO

 @Author: nick1484065161
 @Date: 2026/07/20 00:00:00

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
#pragma once

#ifndef _TCM_PRESCRIPTION_WITHDRAW_DTO_H_
#define _TCM_PRESCRIPTION_WITHDRAW_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class TcmPrescriptionWithdrawDTO : public oatpp::DTO
{
	DTO_INIT(TcmPrescriptionWithdrawDTO, DTO);
	// 住院就诊记录 ID
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("tcm-prescription.field.encounter-id"), true, "335444239397687296");

	// 患者 ID
	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("tcm-prescription.field.patient-id"), true, "335441797981409280");

	// 处方操作类型
	API_DTO_FIELD(String, action, ZH_WORDS_GETTER("tcm-prescription.field.action"), true, "WITHDRAW");

	// 需要撤回的药品医嘱 ID 集合
	API_DTO_FIELD(List<String>, medicationRequestIds, ZH_WORDS_GETTER("tcm-prescription.field.medication-request-ids"), true, {});

	// 中药处方分组标识
	API_DTO_FIELD(String, prescriptionGroupKey, ZH_WORDS_GETTER("tcm-prescription.field.prescription-group-key"), true, "tcm-rx-example-group");

	// 中药处方编号
	API_DTO_FIELD(String, prescriptionNo, ZH_WORDS_GETTER("tcm-prescription.field.prescription-no"), true, "CP2026-07-150001");

	// 操作范围
	API_DTO_FIELD(String, scope, ZH_WORDS_GETTER("tcm-prescription.field.scope"), true, "PRESCRIPTION");

 };

#include OATPP_CODEGEN_END(DTO)

#endif // !_TCM_PRESCRIPTION_WITHDRAW_DTO_H_
