#include "stdafx.h"
#include "PatientController.h"
#include "StringUtil.h"
#include "../../service/pat/PatService.h"


PatientJsonVO::Wrapper PatientController::execQueryById(const String& id) {
	return PatientJsonVO::createShared();
}
// 3.3 演示新增数据
StringJsonVO::Wrapper PatientController::execAddPatient(const PatientAddDTO::Wrapper& dto, const PayloadDTO& payload) {
	return StringJsonVO::createShared();
}
// 3.3 演示修改数据
StringJsonVO::Wrapper PatientController::execModifyPatient(const PatientDTO::Wrapper& dto, const PayloadDTO& payload) {
	return StringJsonVO::createShared();
}

// Remove patient (soft-delete by ID)
BooleanJsonVO::Wrapper PatientController::execRemovePatient(const PatientRemoveDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = BooleanJsonVO::createShared();
	if (!dto || !dto->id || dto->id->empty())
	{
		response->init(false, RS_PARAMS_INVALID);
		return response;
	}
	std::string id = dto->id.getValue("");
	StringUtil::trim(&id);
	if (id.empty() || id.size() > 64)
	{
		response->init(false, RS_PARAMS_INVALID);
		return response;
	}
	PatService patService;
	bool success = patService.removePatient(id, payload);
	if (!success)
	{
		response->fail(false);
		return response;
	}
	response->success(true);
	return response;
}
