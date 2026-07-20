#pragma once

#include "../../GlobalInclude.h"
#include "../../dto/pay_manage/GetPatientPayRecordsDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientPayRecordsPageJsonVO : public JsonVO<GetPatientPayRecordsPageDTO::Wrapper> {
	DTO_INIT(GetPatientPayRecordsPageJsonVO, JsonVO<GetPatientPayRecordsPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)