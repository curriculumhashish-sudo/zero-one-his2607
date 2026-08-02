#include "stdafx.h"
#include "PrintRegisterReceiptController.h"
#include "../../service/register/PrintRegisterReceiptService.h"

PrintRegisterReceiptVO::Wrapper PrintRegisterReceiptController::execPrintRegisterReceipt(const PrintRegisterReceiptDTO::Wrapper& dto, const PayloadDTO& payload)
{
	PrintRegisterReceiptService service;
	auto result = PrintRegisterReceiptVO::createShared();
	result->success(dto);
	return result;
}
