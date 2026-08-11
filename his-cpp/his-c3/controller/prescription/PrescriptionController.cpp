#include "stdafx.h"
#include "PrescriptionController.h"
#include "../../service/prescription/PrescriptionService.h"

// Query historical prescriptions.
PrescriptionHistoryListJsonVO::Wrapper PrescriptionController::execQueryHistory(const PrescriptionHistoryQuery::Wrapper& query) {
	auto result = PrescriptionService().listAllHistoryPrescription(query);
	auto jvo = PrescriptionHistoryListJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

// Query current prescriptions.
PrescriptionCurrentListJsonVO::Wrapper PrescriptionController::execQueryCurrent(const PrescriptionCurrentQuery::Wrapper& query) {
	auto result = PrescriptionService().listAllCurrentPrescription(query);
	auto jvo = PrescriptionCurrentListJsonVO::createShared();
	jvo->success(result);
	return jvo;
}

// Add a new prescription.
PrescriptionNewResponseVO::Wrapper PrescriptionController::execAddPrescription(const PrescriptionNewDTO::Wrapper& dto, const PayloadDTO& payload) {
	auto jvo = PrescriptionNewResponseVO::createShared();
	auto res = PrescriptionNewResponseDTO::createShared();
	jvo->data = res;
	res = PrescriptionService().savePrescription(dto, payload);

	if (res->message != "add fail" && false==res->success)
	{
		jvo->init(res, RS_PARAMS_INVALID);
	}else if (res->success){
		jvo->success(res);
	}else{
		jvo->fail(res);
	}
	return jvo;
}

// Soft-delete prescriptions by medication request ids.
PrescriptionDeleteResponseVO::Wrapper PrescriptionController::execRemovePrescription(const PrescriptionDeleteDTO::Wrapper& dto) {
	auto jvo = PrescriptionDeleteResponseVO::createShared();
	auto res = PrescriptionDeleteResponseDTO::createShared();
	jvo->data = res;
	res = PrescriptionService().removePrescription(dto);

	if (res->message != "delete fail" && false == res->success)
	{
		jvo->init(res, RS_PARAMS_INVALID);
	}else if (res->success) {
		jvo->success(res);
	}
	else{
		jvo->fail(res);
	}
	return jvo;
}