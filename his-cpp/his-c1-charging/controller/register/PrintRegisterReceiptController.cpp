#include "stdafx.h"
#include "PrintRegisterReceiptController.h"
#include "../../service/register/PrintRegisterReceiptService.h"

PrintRegisterReceiptVO::Wrapper PrintRegisterReceiptController::execPrintRegisterReceipt(const PrintRegisterReceiptDTO::Wrapper& dto, const PayloadDTO& payload)
{
	PrintRegisterReceiptService service;
	
	// Call service to handle print logic
	bool printSuccess = true; // Assume success by default
	std::string errorMessage = "";
	std::string printerStatus = "success";
	
	// TODO: Call actual service method to handle print logic
	// int result = service.confirmPrint(dto);
	// printSuccess = (result > 0);
	
	// Create response VO with print status
	auto result = PrintRegisterReceiptVO::createShared();
	result->printRequest = dto;
	result->printSuccess = printSuccess;
	result->errorMessage = errorMessage;
	result->printerStatus = printerStatus;
	
	if (printSuccess) {
		result->code = 200;
		result->message = ZH_WORDS_GETTER("base.success");
	} else {
		result->code = 500;
		result->message = ZH_WORDS_GETTER("base.failed");
	}
	
	return result;
}
