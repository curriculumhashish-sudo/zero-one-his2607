#pragma once

#ifndef _ORDER_EXECUTE_VO_
#define _ORDER_EXECUTE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/medicalorderexecution/OrderExecuteDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CancelExecutionJsonVO : public JsonVO<CancelExecutionResultDTO::Wrapper>
{
	DTO_INIT(CancelExecutionJsonVO, JsonVO<CancelExecutionResultDTO::Wrapper>);
public:
	CancelExecutionJsonVO()
	{
		data = CancelExecutionResultDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORDER_EXECUTE_VO_
