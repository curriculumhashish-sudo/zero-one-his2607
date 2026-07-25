#include "stdafx.h"
#include "PatientController.h"


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
