#include "stdafx.h"
#include "MedicalRecordController.h"
#include "../../service/inpatient-record/MedicalRecordService.h"
using namespace std;

ByTitlePageJsonVO::Wrapper MedicalRecordController::executeQueryByTitle(const MedicalRecordQueryByTitle::Wrapper& query)
{
	

	//响应结果
	auto jvo = ByTitlePageJsonVO::createShared();
	if (!query )
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	//查询数据
	auto result = MedicalRecordService().listAll(query);
	
	jvo->success(result);
	return jvo;
}
ByIdJsonVO::Wrapper MedicalRecordController::executeQueryById(const MedicalRecordQueryById::Wrapper& query)
{
	//响应结果
	auto jvo = ByIdJsonVO::createShared();
	if (!query->id)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	//查询数据
	auto result = MedicalRecordService().getById(query);

	jvo->success(result);
	return jvo;
}

StringJsonVO::Wrapper MedicalRecordController::execAddMedicalRecord(const MedicalRecordAddDTO::Wrapper& dto, const PayloadDTO& payload)
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
	std::string id = MedicalRecordService().saveData(dto);
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

StringJsonVO::Wrapper MedicalRecordController::execModifyMedicalRecord(const MedicalRecordAddDTO::Wrapper& dto, const PayloadDTO& payload)
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
	if (MedicalRecordService().updateData(dto)) {
		jvo->success(dto->id);
	}
	else
	{
		jvo->fail(dto->id);
	}
	// 响应结果
	return jvo;
}