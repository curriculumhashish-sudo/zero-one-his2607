#include "stdafx.h"
#include "DeptBatchStatusController.h"
#include "../../service/dept/DeptBatchStatusService.h"
#include "oatpp/core/parser/ParsingError.hpp"

std::string DeptBatchStatusController::mapDeserializeError(const std::string& rawMsg)
{
	// 数字未加引号：期望字符串 "
	if (rawMsg.find("preparseString") != std::string::npos ||
		rawMsg.find("'\"' - expected") != std::string::npos)
	{
		return ZH_WORDS_GETTER("dept.batch-status.validate.id-not-digit");
	}

	// deptIdList 缺失值 / 不是数组：期望 [
	if (rawMsg.find("deserializeCollection") != std::string::npos ||
		rawMsg.find("'[' - expected") != std::string::npos)
	{
		return ZH_WORDS_GETTER("dept.batch-status.validate.id-list-required");
	}

	// targetStatus 非法整数
	if (rawMsg.find("ERROR_INVALID_INTEGER") != std::string::npos ||
		rawMsg.find("Invalid Integer") != std::string::npos)
	{
		return ZH_WORDS_GETTER("dept.batch-status.validate.status-required");
	}

	return ZH_WORDS_GETTER("dept.batch-status.validate.body-invalid");
}

DeptBatchStatusJsonVO::Wrapper DeptBatchStatusController::execBatchToggleDeptStatus(
	const oatpp::String& body,
	const PayloadDTO& payload)
{
	auto respVo = DeptBatchStatusJsonVO::createShared();

	// 1. 请求体为空
	if (!body || body->empty())
	{
		respVo->init(RS_PARAMS_INVALID);
		respVo->message = ZH_WORDS_GETTER("dept.batch-status.validate.id-list-required");
		return respVo;
	}

	// 2. 安全反序列化：类型错误在此捕获，不再抛成 500/9994
	DeptBatchStatusDTO::Wrapper dto = nullptr;
	try
	{
		dto = getDefaultObjectMapper()->readFromString<DeptBatchStatusDTO::Wrapper>(body);
	}
	catch (const oatpp::parser::ParsingError& e)
	{
		std::string raw = e.getMessage() ? e.getMessage()->c_str() : e.what();
		respVo->init(RS_PARAMS_INVALID);
		respVo->message = mapDeserializeError(raw);
		return respVo;
	}
	catch (const std::exception& e)
	{
		respVo->init(RS_PARAMS_INVALID);
		respVo->message = mapDeserializeError(e.what());
		return respVo;
	}

	if (!dto)
	{
		respVo->init(RS_PARAMS_INVALID);
		respVo->message = ZH_WORDS_GETTER("dept.batch-status.validate.body-invalid");
		return respVo;
	}

	// 3. 业务校验（必填、枚举、重复ID等）
	std::string errMsg = dto->validate();
	if (!errMsg.empty())
	{
		respVo->init(RS_PARAMS_INVALID);
		respVo->message = errMsg;
		return respVo;
	}

	// 4. 绑定登录用户并执行业务
	dto->setPayload(&payload);
	DeptBatchStatusService batchService;
	BatchResult result = batchService.batchToggleDeptStatus(dto);

	// 5. 组装响应
	if (result.success)
	{
		respVo->init(RS_SUCCESS);
		respVo->message = (dto->targetStatus == 1)
			? ZH_WORDS_GETTER("dept.batch-status.success.enable")
			: ZH_WORDS_GETTER("dept.batch-status.success.disable");
	}
	else
	{
		respVo->init(RS_FAIL);
		respVo->message = result.errorMsg.empty()
			? ZH_WORDS_GETTER("dept.batch-status.fail.empty")
			: result.errorMsg;
	}

	return respVo;
}