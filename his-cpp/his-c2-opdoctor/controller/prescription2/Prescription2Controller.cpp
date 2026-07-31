#include "stdafx.h"
#include "Prescription2Controller.h"
#include "service/prescription2/MedicalAdviceService.h"

SignMedicalAdviceJsonVO::Wrapper Prescription2Controller::execSignMedicalAdvice(const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = SignMedicalAdviceJsonVO::createShared();
	if (!dto || !dto->medicalAdviceIds || dto->medicalAdviceIds->empty())
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	// 将事务性状态流转委托给服务层处理。
	auto signedIds = MedicalAdviceService().signMedicalAdvice(dto, payload);
	if (signedIds.empty())
	{
		response->fail(nullptr);
		return response;
	}

	auto data = oatpp::List<oatpp::String>::createShared();
	for (const auto& id : signedIds)
	{
		data->push_back(id.c_str());
	}
	response->success(data);
	return response;
}

WithdrawMedicalAdviceJsonVO::Wrapper Prescription2Controller::execWithdrawMedicalAdvice(const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = WithdrawMedicalAdviceJsonVO::createShared();
	if (!dto || !dto->medicalAdviceIds || dto->medicalAdviceIds->empty())
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	// 将事务性状态流转委托给服务层处理。
	auto withdrawnIds = MedicalAdviceService().withdrawMedicalAdvice(dto, payload);
	if (withdrawnIds.empty())
	{
		response->fail(nullptr);
		return response;
	}

	auto data = oatpp::List<oatpp::String>::createShared();
	for (const auto& id : withdrawnIds)
	{
		data->push_back(id.c_str());
	}
	response->success(data);
	return response;
}

PrintApplicationFormJsonVO::Wrapper Prescription2Controller::execPrintApplicationForm(const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = PrintApplicationFormJsonVO::createShared();
	if (!dto || !dto->serviceRequestId || dto->serviceRequestId->empty())
	{
		response->init(nullptr, RS_PARAMS_INVALID);
		return response;
	}

	// 服务层记录成功的打印请求并返回其标识。
	const std::string serviceRequestId = MedicalAdviceService().printApplicationForm(dto, payload);
	if (serviceRequestId.empty())
	{
		response->fail(nullptr);
		return response;
	}

	response->success(serviceRequestId.c_str());
	return response;
}
