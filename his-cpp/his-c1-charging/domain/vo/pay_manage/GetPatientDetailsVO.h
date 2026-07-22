#pragma once

#include "../../GlobalInclude.h"
#include "domain/dto/pay_manage/GetPatientDetailsDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientDetailsVO : public JsonVO<GetPatientDetailsDTO::Wrapper>
{
    DTO_INIT(GetPatientDetailsVO, JsonVO<GetPatientDetailsDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
