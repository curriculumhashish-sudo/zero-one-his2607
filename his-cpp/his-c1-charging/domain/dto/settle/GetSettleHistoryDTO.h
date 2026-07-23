#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: ovo
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
#ifndef _GET_SETTLE_HISTORY_DTO_H_
#define _GET_SETTLE_HISTORY_DTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SettlePaymentDetailDTO : public oatpp::DTO
{
	DTO_INIT(SettlePaymentDetailDTO, DTO);

	API_DTO_FIELD(String, paymentDetailId, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDetailDTO.paymentDetailId.description"), true, "330100000000000101");
	API_DTO_FIELD(String, payEnum, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDetailDTO.payEnum.description"), true, "CASH");
	API_DTO_FIELD(Float64, amount, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDetailDTO.amount.description"), true, 480.5);
	API_DTO_FIELD(Float64, returnAmount, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDetailDTO.returnAmount.description"), false, 0.0);
	API_DTO_FIELD(String, payTransNo, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDetailDTO.payTransNo.description"), false, "CASH202607200001");
	API_DTO_FIELD(String, payTransDate, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDetailDTO.payTransDate.description"), true, "2026-07-20 11:00:00");
	API_DTO_FIELD(String, resultEnum, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDetailDTO.resultEnum.description"), true, "SUCCESS");
};

class SettleHistoryItemDTO : public oatpp::DTO
{
	DTO_INIT(SettleHistoryItemDTO, DTO);

	API_DTO_FIELD(String, settlementId, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.settlementId.description"), true, "330100000000000001");
	API_DTO_FIELD(String, paymentNo, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.paymentNo.description"), true, "IPAY202607200001");
	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.patientId.description"), true, "329874660197928960");
	API_DTO_FIELD(String, patientName, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.patientName.description"), true, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.patientName.value"));
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.encounterId.description"), true, "329874660147597312");
	API_DTO_FIELD(String, encounterBusNo, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.encounterBusNo.description"), true, "IP202607200001");
	API_DTO_FIELD(String, kindEnum, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.kindEnum.description"), true, "SETTLEMENT");
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.statusEnum.description"), true, "CONFIRMED");
	API_DTO_FIELD(Float64, displayAmount, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.displayAmount.description"), true, 480.5);
	API_DTO_FIELD(Float64, tenderedAmount, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.tenderedAmount.description"), true, 500.0);
	API_DTO_FIELD(Float64, returnedAmount, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.returnedAmount.description"), true, 19.5);
	API_DTO_FIELD(String, billDate, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.billDate.description"), true, "2026-07-20 11:00:00");
	API_DTO_FIELD(String, invoiceId, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.invoiceId.description"), false, "330100000000000201");
	API_DTO_FIELD(String, setlId, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.setlId.description"), false, "SETL202607200001");
	API_DTO_FIELD(Vector<Object<SettlePaymentDetailDTO>>, paymentDetails, ZH_WORDS_GETTER("ovo.charging.settle.SettleHistoryItemDTO.paymentDetails.description"), true, {});

public:
	SettleHistoryItemDTO()
	{
		paymentDetails = Vector<Object<SettlePaymentDetailDTO>>::createShared();
		paymentDetails->push_back(SettlePaymentDetailDTO::createShared());
	}
};

class GetSettleHistoryPageDTO : public PageDTO<SettleHistoryItemDTO::Wrapper>
{
	DTO_INIT(GetSettleHistoryPageDTO, PageDTO<SettleHistoryItemDTO::Wrapper>);

public:
	GetSettleHistoryPageDTO()
	{
		pageIndex = 1;
		pageSize = 10;
		total = 1;
		pages = 1;
		rows = List<SettleHistoryItemDTO::Wrapper>::createShared();
		rows->push_back(SettleHistoryItemDTO::createShared());
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_GET_SETTLE_HISTORY_DTO_H_
