#include "stdafx.h"
#include "DiagnosisController.h"
#include "service/diagnosis/DiagnosisService.h"




DiagnosisAddResultVO::Wrapper DiagnosisController::execAddDiagnosis(const DiagnosisAddDTO::Wrapper& dto, const PayloadDTO& payload)
{

	// 定义返回数据对象
	auto jvo = DiagnosisAddResultVO::createShared();

	// 参数校验
	std::string errmsg = dto->validate();
	if (errmsg != "")
	{
		auto resultData = DiagnosisAddResultDTO::createShared();

		jvo->init(resultData, RS_PARAMS_INVALID);
		return jvo;
	}
	
	// 执行数据新增
	dto->setPayload(&payload);
	auto result = DiagnosisService().saveData(dto);

	jvo->success(result);
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
/*
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
}*/





BooleanJsonVO::Wrapper DiagnosisController::execRemoveDiagnosis(const String& id)
{
    auto jvo = BooleanJsonVO::createShared();
    
    if (id->empty()) {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
    
    // 1. 查询获取完整数据
    auto res = DiagnosisService().getById(id);
    if (!res) {
        jvo->init(nullptr, RS_FAIL);
        return jvo;
    }
    
    // 2. 构造 RemoveDTO 
    auto dto = DiagnosisRemoveDTO::createShared();
    dto->id = res->id;
    dto->conditionId = res->conditionId;
    
	if (!dto->id || dto->id->empty() || !dto->conditionId || dto->conditionId->empty()) 
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
    // 3. 执行删除
    if (DiagnosisService().removeData(dto)) {
        jvo->success(true);
    } else {
        jvo->fail(false);
    }
    
    return jvo;
}

