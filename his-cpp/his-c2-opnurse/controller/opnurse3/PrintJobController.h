#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#ifndef _PRINT_JOB_CONTROLLER_
#define _PRINT_JOB_CONTROLLER_

#include "ApiHelper.h"
#include "domain/vo/opnurse3/PrintJobVO.h"
#include "domain/dto/opnurse3/PrintApiDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

class PrintJobController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(PrintJobController);
public:
	ENDPOINT_INFO(printJob) {
		API_DEF_ADD_AUTH();
		info->summary = ZH_WORDS_GETTER("print.print.summary");
		API_DEF_ADD_RSP_JSON(oatpp::Object<PrintResultJsonVO>);
	}
	// POST /opnurse3/print
	ENDPOINT(API_M_POST, "/opnurse3/print", printJob, BODY_DTO(PrintRequestDTO::Wrapper, dto), API_HANDLER_AUTH_PARAME) {
		return createDtoResponse(Status::CODE_200, executePrint(dto));
	}

private:
	PrintResultJsonVO::Wrapper executePrint(const PrintRequestDTO::Wrapper& dto);
};

#include OATPP_CODEGEN_END(ApiController)
#endif // !_PRINT_JOB_CONTROLLER_
