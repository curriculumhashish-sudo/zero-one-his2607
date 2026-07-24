#pragma once
#ifndef _SUPPLEMENTCHARGE_VO_
#define _SUPPLEMENTCHARGE_VO_

#include "../../../GlobalInclude.h"
#include "../../../dto/inpatientbilling/supplementcharge/SupplementChargeDTO.h"	
#include OATPP_CODEGEN_BEGIN(DTO)


/**
 * 补费响应VO，用于响应给客户端的Json对象
 */
class SupplementChargeVO : public JsonVO<SupplementChargeDTO::Wrapper> {
	DTO_INIT(SupplementChargeVO, JsonVO<SupplementChargeDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SUPPLEMENTCHARGE_VO_