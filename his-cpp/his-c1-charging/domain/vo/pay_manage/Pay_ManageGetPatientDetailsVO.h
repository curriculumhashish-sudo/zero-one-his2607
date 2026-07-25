#pragma once

#include "../../GlobalInclude.h"
#include "domain/dto/pay_manage/Pay_ManageGetPatientDetailsDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientDetailsVO : public JsonVO<Pay_ManageGetPatientDetailsDTO::Wrapper>
{
    DTO_INIT(Pay_ManageGetPatientDetailsVO, JsonVO<Pay_ManageGetPatientDetailsDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
