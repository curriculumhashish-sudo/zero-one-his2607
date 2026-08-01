#include "stdafx.h"
#include "QuitNumberController.h"
#include "../../service/register/QuitNumberService.h"

QuitNumberJsonVO::Wrapper QuitNumberController::execConfirmQuitNumber(const QuitNumberDTO::Wrapper& dto, const PayloadDTO& payload)
{
	
	QuitNumberService service;
	bool ok = service.confirmQuitNumber(dto, payload);
	auto vo = QuitNumberJsonVO::createShared();
	if (ok)
	{
		vo->success(dto);
	}
	else
	{
		vo->fail(dto);
	}
	return vo;
}
