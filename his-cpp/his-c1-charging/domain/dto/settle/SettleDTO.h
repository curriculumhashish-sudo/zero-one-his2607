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
#ifndef _SETTLE_DTO_H_
#define _SETTLE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SettlePaymentDTO : public oatpp::DTO
{
	DTO_INIT(SettlePaymentDTO, DTO);

	API_DTO_FIELD(String, payEnum, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDTO.payEnum.description"), true, "CASH");
	API_DTO_FIELD(Float64, amount, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDTO.amount.description"), true, 480.5);
	API_DTO_FIELD(String, payTransNo, ZH_WORDS_GETTER("ovo.charging.settle.SettlePaymentDTO.payTransNo.description"), false, "CASH202607200001");
};

class SettleRequestDTO : public oatpp::DTO
{
	DTO_INIT(SettleRequestDTO, DTO);

	API_DTO_FIELD(String, patientId, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.patientId.description"), true, "329874660197928960");
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.encounterId.description"), true, "329874660147597312");
	API_DTO_FIELD(String, accountId, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.accountId.description"), true, "330100000000000301");
	API_DTO_FIELD(Vector<String>, chargeItemIds, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.chargeItemIds.description"), true, {});
	API_DTO_FIELD(Vector<Object<SettlePaymentDTO>>, paymentDetails, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.paymentDetails.description"), true, {});
	API_DTO_FIELD(Float64, displayAmount, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.displayAmount.description"), true, 480.5);
	API_DTO_FIELD(String, contractNo, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.contractNo.description"), false, "SELF_PAY");
	API_DTO_FIELD(String, billDate, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.billDate.description"), true, "2026-07-20 11:00:00");
	API_DTO_FIELD(String, remark, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.remark.description"), false, ZH_WORDS_GETTER("ovo.charging.settle.SettleRequestDTO.remark.value"));

public:
	SettleRequestDTO()
	{
		chargeItemIds = Vector<String>::createShared();
		chargeItemIds->push_back("329906419492982785");
		paymentDetails = Vector<Object<SettlePaymentDTO>>::createShared();
		paymentDetails->push_back(SettlePaymentDTO::createShared());
	}
};

class SettleResultDTO : public oatpp::DTO
{
	DTO_INIT(SettleResultDTO, DTO);

	API_DTO_FIELD(String, paymentId, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.paymentId.description"), true, "330100000000000001");
	API_DTO_FIELD(String, paymentNo, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.paymentNo.description"), true, "IPAY202607200001");
	API_DTO_FIELD(String, preSettleId, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.preSettleId.description"), false, "330100000000000401");
	API_DTO_FIELD(String, setlId, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.setlId.description"), false, "SETL202607200001");
	API_DTO_FIELD(String, invoiceId, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.invoiceId.description"), false, "330100000000000201");
	API_DTO_FIELD(Float64, displayAmount, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.displayAmount.description"), true, 480.5);
	API_DTO_FIELD(String, statusEnum, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.statusEnum.description"), true, "CONFIRMED");
	API_DTO_FIELD(String, settledAt, ZH_WORDS_GETTER("ovo.charging.settle.SettleResultDTO.settledAt.description"), true, "2026-07-20 11:00:01");
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SETTLE_DTO_H_
