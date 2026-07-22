#include "stdafx.h"
#include "PrescriptionController.h"

//历史处方执行函数
PrescriptionHistoryListJsonVO::Wrapper PrescriptionController::execQueryHistory(const PrescriptionHistoryQuery::Wrapper& query) {
	return {};
}
//现有处方执行函数
PrescriptionCurrentListJsonVO::Wrapper PrescriptionController::execQueryCurrent(const PrescriptionCurrentQuery::Wrapper& query) {
	return{};
}
//新增处方接收函数
PrescriptionNewResponseVO::Wrapper PrescriptionController::execAddPrescription(const PrescriptionNewDTO::Wrapper& dto, const PayloadDTO& payload) {
	return{};
}
//删除处方执行函数
PrescriptionDeleteResponseVO::Wrapper PrescriptionController::execRemovePrescription(const PrescriptionDeleteDTO::Wrapper& ids) {
	return{};
}