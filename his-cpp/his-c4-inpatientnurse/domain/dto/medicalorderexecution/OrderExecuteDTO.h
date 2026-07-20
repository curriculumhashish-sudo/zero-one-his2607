#pragma once

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*不执行医嘱DTO对象*/
class OrderNotExecuteDTO : public oatpp::DTO
{
	DTO_INIT(OrderNotExecuteDTO, DTO);

	//id
	DTO_FIELD(String, advice_id);
	DTO_FIELD_INFO(advice_id) {
		info->description = ZH_WORDS_GETTER("medical_advice.notexecute.field.advice_id");
	};
	
	//医嘱类型
	DTO_FIELD(String, advice_type);
	DTO_FIELD_INFO(advice_type) {
		info->description = ZH_WORDS_GETTER("medical_advice.notexecute.field.advice_type");
	};

	//计划时间点
	DTO_FIELD(String, plan_time);
	DTO_FIELD_INFO(plan_time) {
		info->description = ZH_WORDS_GETTER("medical_advice.notexecute.field.plan_time");
	};

public:
	// 数据校验
	std::string validate()
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
};

/*取消执行医嘱DTO中item对象*/
class CancelExecutionItemDTO : public oatpp::DTO 
{
	DTO_INIT(CancelExecutionItemDTO, DTO);

	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.field.items.encounterId");
	}

	DTO_FIELD(String, procedureId);
	DTO_FIELD_INFO(procedureId) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.field.items.procedureId");
	}
};

/*取消执行医嘱DTO对象*/
class CancelExecutionRequestDTO : public oatpp::DTO 
{
	DTO_INIT(CancelExecutionRequestDTO, DTO);

	/*取消执行item列表*/
	DTO_FIELD(List<CancelExecutionItemDTO::Wrapper>, items);

	/*1取消执行原因*/
	DTO_FIELD(String, statusReasonText);
	DTO_FIELD_INFO(statusReasonText) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.field.statusReasonText");
	}

public:
	std::string validate()
	{
		/*字段非空基本校验，取消执行item列表不能多于50项*/
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

/*取消执行返回结果DTO*/
class CancelExecutionResultDTO : public oatpp::DTO
{
	DTO_INIT(CancelExecutionResultDTO, DTO);

	/*是否成功(请求列表中一条失败整体就失败)*/
	DTO_FIELD(Boolean, success);
	DTO_FIELD_INFO(success) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.result.success");
	}

	/*被取消执行记录id*/
	DTO_FIELD(List<String>, cancelledProcedureIds);
	DTO_FIELD_INFO(cancelledProcedureIds) {
		info->description = ZH_WORDS_GETTER("medical_advice.cancelexecute.result.cancelledProcedureIds");
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
};


#include OATPP_CODEGEN_END(DTO)
