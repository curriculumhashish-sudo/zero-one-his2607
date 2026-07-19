#pragma once

#ifndef _DETAIL_QEURY_VO_
#define _DETAIL_QEURY_VO_

#include "../../GlobalInclude.h"
#include "../../domain/dto/detail/queryDetailDTO.h"
#

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  测试：显示JsonVO，用于响应给客户端的Json对象
 */
class DetailJsonVO : public JsonVO<DetailDTO::Wrapper> {
	DTO_INIT(DetailJsonVO, JsonVO<DetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SAMPLE_VO_