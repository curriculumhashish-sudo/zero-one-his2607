#pragma once
#ifndef _SUPPLEMENTORDER_VO_
#define _SUPPLEMENTORDER_VO_

#include "../../../GlobalInclude.h"
#include "../../../dto/inpatientbilling/supplementcharge/SupplementOrderDTO.h"	
#include OATPP_CODEGEN_BEGIN(DTO)


/**
 * 分页显示JsonVO，用于响应给客户端的Json对象
 */
class SupplementOrderVO : public JsonVO<SupplementOrderDTO::Wrapper> {
	DTO_INIT(SupplementOrderVO, JsonVO<SupplementOrderDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SUPPLEMENTORDER_VO_