#pragma once

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Request item used to skip medical-order execution.
class OrderNotExecuteDTO : public oatpp::DTO
{
	DTO_INIT(OrderNotExecuteDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, advice_id,
		ZH_WORDS_GETTER("medical_advice.notexecute.field.advice_id"), true);
	
	API_DTO_FIELD_REQUIRE(String, advice_type,
		ZH_WORDS_GETTER("medical_advice.notexecute.field.advice_type"), true);

	API_DTO_FIELD_REQUIRE(String, plan_time,
		ZH_WORDS_GETTER("medical_advice.notexecute.field.plan_time"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->advice_id = "337907884548657152";
		dto->advice_type = "MED_REQUEST";
		dto->plan_time = "2026-07-24 08:00:00";
		return dto;
	}

	// Validate the request item.
	std::string validate() const
	{
		if (!advice_id || advice_id->empty() || advice_id->size() > 64)
			return "advice_id invalidate.";

		if (!advice_type || advice_type->empty())
			return "advice_type invalidate.";

		if (advice_type != "MED_REQUEST" && advice_type != "SERVICE_REQUEST")
			return "advice_type only supports MED_REQUEST or SERVICE_REQUEST.";

		if (!plan_time || plan_time->empty())
			return "plan_time invalidate.";

		return "";
	}

	static std::string validate(const OrderNotExecuteDTO::Wrapper& dto)
	{
		if (!dto)
			return "order not execute item is required.";

		return dto->validate();
	}
};

// Medical-order cancellation item.
class CancelExecutionItemDTO : public oatpp::DTO 
{
	DTO_INIT(CancelExecutionItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.field.items.encounterId"), true);

	API_DTO_FIELD_REQUIRE(String, procedureId,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.field.items.procedureId"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->encounterId = "337907884548657152";
		dto->procedureId = "PROC202607240001";
		return dto;
	}
};

class CancelExecutionRequestDTO : public oatpp::DTO 
{
	DTO_INIT(CancelExecutionRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<CancelExecutionItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.field.itemList"), true);

	API_DTO_FIELD_REQUIRE(String, statusReasonText,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.field.statusReasonText"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->items = {CancelExecutionItemDTO::createExample()};
		dto->statusReasonText = ZH_WORDS_GETTER("business-text.execution.deferred-reason");
		return dto;
	}

	std::string validate()
	{
		if (!items || items->empty())
			return "items is required.";
		if (items->size() > 50)
			return "items size must be <= 50.";
		if (!statusReasonText || statusReasonText->empty())
			return "statusReasonText is required.";

		for (auto& item : *items)
		{
			if (!item)
				return "item is required.";

			if (!item->encounterId || item->encounterId->empty())
				return "encounterId is required.";

			if (!item->procedureId || item->procedureId->empty())
				return "procedureId is required.";
		}

		return "";
	}
};

class CancelExecutionResultDTO : public oatpp::DTO
{
	DTO_INIT(CancelExecutionResultDTO, DTO);

	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.result.success"), true);

	API_DTO_FIELD_REQUIRE(List<String>, rebuiltSummaryNos,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.result.rebuiltSummaryNos"), true);

	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.result.message"), false);

public:
	CancelExecutionResultDTO()
	{
		success = false;
		rebuiltSummaryNos = List<String>::createShared();
	}
};


#include OATPP_CODEGEN_END(DTO)
