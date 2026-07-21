#include "stdafx.h"
#include "prescriptionController.h"

SignMedicalAdviceJsonVO::Wrapper PrescriptionController::execSignMedicalAdvice(const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return SignMedicalAdviceJsonVO::Wrapper();
}

WithdrawMedicalAdviceJsonVO::Wrapper PrescriptionController::execWithdrawMedicalAdvice(const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return WithdrawMedicalAdviceJsonVO::Wrapper();
}

PrintApplicationFormJsonVO::Wrapper PrescriptionController::execPrintApplicationForm(const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return PrintApplicationFormJsonVO::Wrapper();
}
