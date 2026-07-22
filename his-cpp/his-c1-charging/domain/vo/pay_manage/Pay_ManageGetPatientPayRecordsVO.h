#pragma once

#include "../../GlobalInclude.h"
#include "../../dto/pay_manage/Pay_ManageGetPatientPayRecordsDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientPayRecordsPageJsonVO : public JsonVO<Pay_ManageGetPatientPayRecordsPageDTO::Wrapper> {
	DTO_INIT(Pay_ManageGetPatientPayRecordsPageJsonVO, JsonVO<Pay_ManageGetPatientPayRecordsPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)