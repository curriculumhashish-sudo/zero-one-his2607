#include "stdafx.h"
#include "DiagnosisController.h"
#include "service/diagnosis/DiagnosisService.h"




StringJsonVO::Wrapper DiagnosisController::execAddDiagnosis(const DiagnosisAddDTO::Wrapper& dto, const PayloadDTO& payload)
{

	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();

	// 参数校验
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		jvo->init(errmsg, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据新增
	dto->setPayload(&payload);
	std::string id = DiagnosisService().saveData(dto);
	if (id != "") {
		jvo->success(id);
	}
	else
	{
		jvo->fail("");
	}
	//响应结果
	return jvo;
}

StringJsonVO::Wrapper DiagnosisController::execModifyDiagnosis(const DiagnosisModifyDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();
	// 参数校验
	if (!dto->id)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据修改
	dto->setPayload(&payload);
	if (DiagnosisService().updateData(dto)) {
		jvo->success(dto->id);
	}
	else
	{
		jvo->fail(dto->id);
	}
	// 响应结果
	return jvo;
}

StringJsonVO::Wrapper DiagnosisController::execRemoveDiagnosis(const DiagnosisRemoveDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();
	// 参数校验
	if (!dto->id&&!dto->conditionId)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据修改
	dto->setPayload(&payload);
	if (DiagnosisService().removeData(dto)) {
		std::string result = "id: ";
		result += dto->id;
		result += ", conditionId: ";
		result += dto->conditionId;
		jvo->success(result);
	}
	else
	{
		jvo->fail(dto->id);
	}
	// 响应结果
	return jvo;
}




/*
BooleanJsonVO::Wrapper DiagnosisController::execRemoveDiagnosis(const String& id)
{
	// 定义返回数据对象
	auto jvo = BooleanJsonVO::createShared();
	// 参数校验
	if (id->empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 执行数据删除
	if (DiagnosisService().removeData(id)) {
		jvo->success(true);
	}
	else
	{
		jvo->fail(false);
	}
	// 响应结果
	return jvo;
}
*/
