#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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
#ifndef _PAGE_PATIENT_QUEUE_DTO_
#define _PAGE_PATIENT_QUEUE_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientQueueItemDTO : public oatpp::DTO
{
	DTO_INIT(PatientQueueItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, accountId,
		ZH_WORDS_GETTER("encountermanage.field.accountId"), false);
	API_DTO_FIELD_REQUIRE(Float64, accountBalance,
		ZH_WORDS_GETTER("encountermanage.field.accountBalance"), false);
	API_DTO_FIELD_REQUIRE(String, accountBalanceDisplayMode,
		ZH_WORDS_GETTER("encountermanage.field.accountBalanceDisplayMode"), false);
	API_DTO_FIELD_REQUIRE(Float64, prepayNetBalance,
		ZH_WORDS_GETTER("encountermanage.field.prepayNetBalance"), false);
	API_DTO_FIELD_REQUIRE(String, accountBalanceSubText,
		ZH_WORDS_GETTER("encountermanage.field.accountBalanceSubText"), false);
	API_DTO_FIELD_REQUIRE(String, accountBalanceSummaryText,
		ZH_WORDS_GETTER("encountermanage.field.accountBalanceSummaryText"), false);
	API_DTO_FIELD_REQUIRE(Float64, availableBalance,
		ZH_WORDS_GETTER("encountermanage.field.availableBalance"), false);
	API_DTO_FIELD_REQUIRE(String, admissionTime,
		ZH_WORDS_GETTER("encountermanage.field.admissionTime"), false);
	API_DTO_FIELD_REQUIRE(String, ageText,
		ZH_WORDS_GETTER("encountermanage.field.ageText"), false);
	API_DTO_FIELD_REQUIRE(Float64, arrearsAmount,
		ZH_WORDS_GETTER("encountermanage.field.arrearsAmount"), false);
	API_DTO_FIELD_REQUIRE(String, attendingDoctorName,
		ZH_WORDS_GETTER("encountermanage.field.attendingDoctorName"), false);
	API_DTO_FIELD_REQUIRE(String, bedId,
		ZH_WORDS_GETTER("encountermanage.field.bedId"), false);
	API_DTO_FIELD_REQUIRE(String, bedName,
		ZH_WORDS_GETTER("encountermanage.field.bedName"), false);
	API_DTO_FIELD_REQUIRE(String, diagnosisSummary,
		ZH_WORDS_GETTER("encountermanage.field.diagnosisSummary"), false);
	API_DTO_FIELD_REQUIRE(List<String>, diagnosisList,
		ZH_WORDS_GETTER("encountermanage.field.diagnosisList"), false);
	API_DTO_FIELD_REQUIRE(String, encounterBusNo,
		ZH_WORDS_GETTER("encountermanage.field.encounterBusNo"), false);
	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("encountermanage.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, endTime,
		ZH_WORDS_GETTER("encountermanage.field.endTime"), false);
	API_DTO_FIELD_REQUIRE(String, genderEnum,
		ZH_WORDS_GETTER("encountermanage.field.genderEnum"), false);
	API_DTO_FIELD_REQUIRE(String, genderEnumText,
		ZH_WORDS_GETTER("encountermanage.field.genderEnumText"), false);
	API_DTO_FIELD_REQUIRE(String, houseName,
		ZH_WORDS_GETTER("encountermanage.field.houseName"), false);
	API_DTO_FIELD_REQUIRE(String, houseId,
		ZH_WORDS_GETTER("encountermanage.field.houseId"), false);
	API_DTO_FIELD_REQUIRE(Boolean, lowBalanceWarning,
		ZH_WORDS_GETTER("encountermanage.field.lowBalanceWarning"), false);
	API_DTO_FIELD_REQUIRE(String, organizationId,
		ZH_WORDS_GETTER("encountermanage.field.organizationId"), false);
	API_DTO_FIELD_REQUIRE(String, organizationName,
		ZH_WORDS_GETTER("encountermanage.field.organizationName"), false);
	API_DTO_FIELD_REQUIRE(Float64, paidAmount,
		ZH_WORDS_GETTER("encountermanage.field.paidAmount"), false);
	API_DTO_FIELD_REQUIRE(String, patientId,
		ZH_WORDS_GETTER("encountermanage.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(String, patientName,
		ZH_WORDS_GETTER("encountermanage.field.patientName"), true);
	API_DTO_FIELD_REQUIRE(Int32, pendingAuditCount,
		ZH_WORDS_GETTER("encountermanage.field.pendingAuditCount"), false);
	API_DTO_FIELD_REQUIRE(Int32, pendingExecuteCount,
		ZH_WORDS_GETTER("encountermanage.field.pendingExecuteCount"), false);
	API_DTO_FIELD_REQUIRE(Float64, pendingSettlementTotal,
		ZH_WORDS_GETTER("encountermanage.field.pendingSettlementTotal"), false);
	API_DTO_FIELD_REQUIRE(String, pinyinCode,
		ZH_WORDS_GETTER("encountermanage.field.pinyinCode"), false);
	API_DTO_FIELD_REQUIRE(String, primaryNurseName,
		ZH_WORDS_GETTER("encountermanage.field.primaryNurseName"), false);
	API_DTO_FIELD_REQUIRE(String, priorityEnum,
		ZH_WORDS_GETTER("encountermanage.field.priorityEnum"), false);
	API_DTO_FIELD_REQUIRE(Boolean, recallEnabled,
		ZH_WORDS_GETTER("encountermanage.field.recallEnabled"), false);
	API_DTO_FIELD_REQUIRE(String, startTime,
		ZH_WORDS_GETTER("encountermanage.field.startTime"), false);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("encountermanage.field.statusEnum"), false);
	API_DTO_FIELD_REQUIRE(String, statusEnumText,
		ZH_WORDS_GETTER("encountermanage.field.statusEnumText"), false);
	API_DTO_FIELD_REQUIRE(Float64, totalAmount,
		ZH_WORDS_GETTER("encountermanage.field.totalAmount"), false);
	API_DTO_FIELD_REQUIRE(String, wardId,
		ZH_WORDS_GETTER("encountermanage.field.wardId"), false);
	API_DTO_FIELD_REQUIRE(String, wardName,
		ZH_WORDS_GETTER("encountermanage.field.wardName"), false);
	API_DTO_FIELD_REQUIRE(String, wubiCode,
		ZH_WORDS_GETTER("encountermanage.field.wubiCode"), false);

public:
	PatientQueueItemDTO()
	{
		diagnosisList = List<String>::createShared();
		accountBalance = 0.0;
		prepayNetBalance = 0.0;
		availableBalance = 0.0;
		arrearsAmount = 0.0;
		lowBalanceWarning = false;
		paidAmount = 0.0;
		pendingAuditCount = 0;
		pendingExecuteCount = 0;
		pendingSettlementTotal = 0.0;
		recallEnabled = false;
		totalAmount = 0.0;
	}
};

class PagePatientQueueDTO : public PageDTO<PatientQueueItemDTO::Wrapper>
{
	DTO_INIT(PagePatientQueueDTO, PageDTO<PatientQueueItemDTO::Wrapper>);

public:
	PagePatientQueueDTO()
	{
		this->pageIndex = 1;
		this->pageSize = 10;
		this->total = static_cast<v_int64>(0);
		this->pages = static_cast<v_int64>(0);
		this->rows = List<PatientQueueItemDTO::Wrapper>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PAGE_PATIENT_QUEUE_DTO_
