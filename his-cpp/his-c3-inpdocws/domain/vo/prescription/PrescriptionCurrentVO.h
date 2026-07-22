#pragma once

#ifndef _PRESCRIPTION_CURRENT_VO_
#define _PRESCRIPTION_CURRENT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/PrescriptionCurrentListDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
* 现有处方显示JsonVO，用于响应给客户端的Json对象
*/
class PrescriptionCurrentListJsonVO : public JsonVO<PrescriptionCurrentListDTO::Wrapper>
{
	DTO_INIT(PrescriptionCurrentListJsonVO, JsonVO<PrescriptionCurrentListDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTION_CURRENT_VO_

