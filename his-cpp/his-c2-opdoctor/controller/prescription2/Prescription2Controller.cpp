#include "stdafx.h"
#include "Prescription2Controller.h"

SignMedicalAdviceJsonVO::Wrapper Prescription2Controller::execSignMedicalAdvice(const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return SignMedicalAdviceJsonVO::Wrapper();
}

WithdrawMedicalAdviceJsonVO::Wrapper Prescription2Controller::execWithdrawMedicalAdvice(const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return WithdrawMedicalAdviceJsonVO::Wrapper();
}

PrintApplicationFormJsonVO::Wrapper Prescription2Controller::execPrintApplicationForm(const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload)
{
	return PrintApplicationFormJsonVO::Wrapper();
}
