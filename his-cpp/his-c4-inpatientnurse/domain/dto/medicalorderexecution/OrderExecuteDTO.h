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
	OrderNotExecuteDTO()
	{
		advice_id = "337907884548657152";
		advice_type = "MED_REQUEST";
		plan_time = "2026-07-24 08:00:00";
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
	CancelExecutionItemDTO()
	{
		encounterId = "337907884548657152";
		procedureId = "PROC202607240001";
	}
};

/*取消执行医嘱DTO请求对象*/
class CancelExecutionRequestDTO : public oatpp::DTO 
{
	DTO_INIT(CancelExecutionRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<CancelExecutionItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.field.items"), true);

	API_DTO_FIELD_REQUIRE(String, statusReasonText,
		ZH_WORDS_GETTER("medical_advice.cancelexecute.field.statusReasonText"), true);

public:
	CancelExecutionRequestDTO()
	{
		items = {CancelExecutionItemDTO::createShared()};
		statusReasonText = ZH_WORDS_GETTER("business-text.execution.deferred-reason");
	}

	std::string validate()
	{
		/*字段非空基本校验，取消执行item列表不能多于50项*/
		if (!items || items->empty())
			return "items is required.";
		if (items->size() > 50)
			return "items size must be <= 50.";
		if (!statusReasonText || statusReasonText->empty())
			return "statusReasonText is required.";

		/*内部字段都不能为空*/
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

/*取消执行返回结果DTO*/
class CancelExecutionResultDTO : public oatpp::DTO
{
	DTO_INIT(CancelExecutionResultDTO, DTO);

	/*是否成功(请求列表中一条失败整体就失败)*/
	DTO_FIELD(Boolean, success);
	DTO_FIELD_INFO(success) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.result.success");
	}

	/*如果取消执行的医嘱涉及药品，该字段就是重新生成的药品汇总单id*/
	DTO_FIELD(List<String>, rebuiltSummaryNos);
	DTO_FIELD_INFO(rebuiltSummaryNos) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.result.rebuiltSummaryNos");
	}

	/*描述执行结果*/
	DTO_FIELD(String, message);
	DTO_FIELD_INFO(message) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.result.message");
	}

public:
	CancelExecutionResultDTO()
	{
		success = true;
		rebuiltSummaryNos = {"SUM202607240001"};
		message = ZH_WORDS_GETTER("business-text.execution.cancel-success");
	}
};


#include OATPP_CODEGEN_END(DTO)
