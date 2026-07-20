#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
 @Date: 2026/07/21 00:00:00

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
#ifndef _PATIENT_QUEUE_DTO_
#define _PATIENT_QUEUE_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientStatisticsDTO : public oatpp::DTO
{
	DTO_INIT(PatientStatisticsDTO, DTO);

	API_DTO_FIELD(Int64, totalCount,
		ZH_WORDS_GETTER("admissiontransfer.field.totalCount"), true, 1);
	API_DTO_FIELD(Int64, pendingAuditCount,
		ZH_WORDS_GETTER("admissiontransfer.field.pendingAuditCount"), true, v_int64(0));
	API_DTO_FIELD(Int64, pendingExecuteCount,
		ZH_WORDS_GETTER("admissiontransfer.field.pendingExecuteCount"), true, v_int64(0));
};

class PatientQueueItemDTO : public oatpp::DTO
{
	DTO_INIT(PatientQueueItemDTO, DTO);

	API_DTO_FIELD(String, accountId,
		ZH_WORDS_GETTER("admissiontransfer.field.accountId"), false,
		"337907884548657153");
	API_DTO_FIELD(Float64, accountBalance,
		ZH_WORDS_GETTER("admissiontransfer.field.accountBalance"), false, 1300.00);
	API_DTO_FIELD(String, accountBalanceDisplayMode,
		ZH_WORDS_GETTER("admissiontransfer.field.accountBalanceDisplayMode"), false,
		"BALANCE");
	API_DTO_FIELD(String, accountBalanceSubText,
		ZH_WORDS_GETTER("admissiontransfer.field.accountBalanceSubText"), false,
		"可用余额");
	API_DTO_FIELD(String, accountBalanceSummaryText,
		ZH_WORDS_GETTER("admissiontransfer.field.accountBalanceSummaryText"), false,
		"余额 1300.00 元");
	API_DTO_FIELD(Float64, availableBalance,
		ZH_WORDS_GETTER("admissiontransfer.field.availableBalance"), false, 1300.00);
	API_DTO_FIELD(String, admissionTime,
		ZH_WORDS_GETTER("admissiontransfer.field.admissionTime"), false,
		"2026-07-21 08:30:00");
	API_DTO_FIELD(String, ageText,
		ZH_WORDS_GETTER("admissiontransfer.field.ageText"), false, "26岁");
	API_DTO_FIELD(Float64, arrearsAmount,
		ZH_WORDS_GETTER("admissiontransfer.field.arrearsAmount"), false, 0.00);
	API_DTO_FIELD(String, attendingDoctorName,
		ZH_WORDS_GETTER("admissiontransfer.field.attendingDoctorName"), false,
		"李医生");
	API_DTO_FIELD(String, bedId,
		ZH_WORDS_GETTER("admissiontransfer.field.bedId"), false,
		"334526696776220672");
	API_DTO_FIELD(String, bedName,
		ZH_WORDS_GETTER("admissiontransfer.field.bedName"), false, "1号床");
	API_DTO_FIELD(String, diagnosisSummary,
		ZH_WORDS_GETTER("admissiontransfer.field.diagnosisSummary"), false,
		"主|感冒");
	API_DTO_FIELD(String, encounterBusNo,
		ZH_WORDS_GETTER("admissiontransfer.field.encounterBusNo"), false,
		"ZY202607210001");
	API_DTO_FIELD(String, encounterId,
		ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true,
		"337907884548657152");
	API_DTO_FIELD(String, endTime,
		ZH_WORDS_GETTER("admissiontransfer.field.endTime"), false,
		"2026-07-25 10:00:00");
	API_DTO_FIELD(String, genderEnum,
		ZH_WORDS_GETTER("admissiontransfer.field.genderEnum"), false, "FEMALE");
	API_DTO_FIELD(String, genderEnumText,
		ZH_WORDS_GETTER("admissiontransfer.field.genderEnumText"), false, "女");
	API_DTO_FIELD(String, houseName,
		ZH_WORDS_GETTER("admissiontransfer.field.houseName"), false, "301");
	API_DTO_FIELD(Boolean, lowBalanceWarning,
		ZH_WORDS_GETTER("admissiontransfer.field.lowBalanceWarning"), false, false);
	API_DTO_FIELD(String, organizationId,
		ZH_WORDS_GETTER("admissiontransfer.field.organizationId"), false,
		"327170091059744768");
	API_DTO_FIELD(String, organizationName,
		ZH_WORDS_GETTER("admissiontransfer.field.organizationName"), false,
		"住院内科");
	API_DTO_FIELD(Float64, paidAmount,
		ZH_WORDS_GETTER("admissiontransfer.field.paidAmount"), false, 1500.00);
	API_DTO_FIELD(String, patientId,
		ZH_WORDS_GETTER("admissiontransfer.field.patientId"), true,
		"326940849401761792");
	API_DTO_FIELD(String, patientName,
		ZH_WORDS_GETTER("admissiontransfer.field.patientName"), true, "小孙");
	API_DTO_FIELD(Int32, pendingAuditCount,
		ZH_WORDS_GETTER("admissiontransfer.field.pendingAuditCount"), false, 0);
	API_DTO_FIELD(Int32, pendingExecuteCount,
		ZH_WORDS_GETTER("admissiontransfer.field.pendingExecuteCount"), false, 0);
	API_DTO_FIELD(Float64, pendingSettlementTotal,
		ZH_WORDS_GETTER("admissiontransfer.field.pendingSettlementTotal"), false, 200.00);
	API_DTO_FIELD(String, pinyinCode,
		ZH_WORDS_GETTER("admissiontransfer.field.pinyinCode"), false, "XS");
	API_DTO_FIELD(String, primaryNurseName,
		ZH_WORDS_GETTER("admissiontransfer.field.primaryNurseName"), false,
		"周女士");
	API_DTO_FIELD(String, priorityEnum,
		ZH_WORDS_GETTER("admissiontransfer.field.priorityEnum"), false, "NORMAL");
	API_DTO_FIELD(Boolean, recallEnabled,
		ZH_WORDS_GETTER("admissiontransfer.field.recallEnabled"), false, false);
	API_DTO_FIELD(String, startTime,
		ZH_WORDS_GETTER("admissiontransfer.field.startTime"), false,
		"2026-07-21 08:30:00");
	API_DTO_FIELD(String, statusEnum,
		ZH_WORDS_GETTER("admissiontransfer.field.statusEnum"), false,
		"IN_PROGRESS");
	API_DTO_FIELD(String, statusEnumText,
		ZH_WORDS_GETTER("admissiontransfer.field.statusEnumText"), false,
		"在院");
	API_DTO_FIELD(Float64, totalAmount,
		ZH_WORDS_GETTER("admissiontransfer.field.totalAmount"), false, 200.00);
	API_DTO_FIELD(String, wardId,
		ZH_WORDS_GETTER("admissiontransfer.field.wardId"), false,
		"334526584536674304");
	API_DTO_FIELD(String, wardName,
		ZH_WORDS_GETTER("admissiontransfer.field.wardName"), false,
		"内科疗区");
	API_DTO_FIELD(String, wubiCode,
		ZH_WORDS_GETTER("admissiontransfer.field.wubiCode"), false, "XS");
};

class PatientQueuePageDTO : public PageDTO<PatientQueueItemDTO::Wrapper>
{
	DTO_INIT(PatientQueuePageDTO, PageDTO<PatientQueueItemDTO::Wrapper>);

public:
	PatientQueuePageDTO()
	{
		this->pageIndex = 1;
		this->pageSize = 10;
		this->total = 1;
		this->pages = 1;
		this->rows = List<PatientQueueItemDTO::Wrapper>::createShared();
		this->rows->push_back(PatientQueueItemDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_QUEUE_DTO_
