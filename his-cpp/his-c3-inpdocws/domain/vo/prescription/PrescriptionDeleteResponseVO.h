#pragma once

#ifndef _PRESCRIPTION_DELETE_RESPONSE_VO_
#define _PRESCRIPTION_DELETE_RESPONSE_VO_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/PrescriptionDeleteResponseDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 删除处方响应JsonVO，用于响应给客户端的Json对象
*/
class PrescriptionDeleteResponseVO : public JsonVO<PrescriptionDeleteResponseDTO::Wrapper>
{
	DTO_INIT(PrescriptionDeleteResponseVO, JsonVO<PrescriptionDeleteResponseDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_DELETE_RESPONSE_VO_

