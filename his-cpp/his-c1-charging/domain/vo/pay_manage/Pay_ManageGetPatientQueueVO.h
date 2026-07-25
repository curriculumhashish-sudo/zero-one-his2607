#pragma once

#include "../../GlobalInclude.h"
#include "../../dto/pay_manage/Pay_ManageGetPatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientQueuePageJsonVO : public JsonVO<Pay_ManageGetPatientQueuePageDTO::Wrapper> {
	DTO_INIT(Pay_ManageGetPatientQueuePageJsonVO, JsonVO<Pay_ManageGetPatientQueuePageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)