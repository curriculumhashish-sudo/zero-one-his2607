#pragma once

#ifndef _PRESCRIPTION_NEW_RESPONSE_VO_
#define _PRESCRIPTION_NEW_RESPONSE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/PrescriptionNewResponseDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 新增处方响应JsonVO，用于响应给客户端的Json对象
*/
class PrescriptionNewResponseVO : public JsonVO<PrescriptionNewResponseDTO::Wrapper>
{
	DTO_INIT(PrescriptionNewResponseVO, JsonVO<PrescriptionNewResponseDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_NEW_RESPONSE_VO_

