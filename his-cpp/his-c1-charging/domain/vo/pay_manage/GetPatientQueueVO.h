#pragma once

#include "../../GlobalInclude.h"
#include "../../dto/pay_manage/GetPatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientQueuePageJsonVO : public JsonVO<GetPatientQueuePageDTO::Wrapper> {
	DTO_INIT(GetPatientQueuePageJsonVO, JsonVO<GetPatientQueuePageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)