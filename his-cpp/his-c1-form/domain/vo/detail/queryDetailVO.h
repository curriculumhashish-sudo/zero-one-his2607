#pragma once

#ifndef _DETAIL_QEURY_VO_
#define _DETAIL_QEURY_VO_

#include "../../GlobalInclude.h"
#include "../../dto/query/detailQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 示例显示JsonVO，用于响应给客户端的Json对象
 */
class DetailJsonVO : public JsonVO<Detail::Wrapper> {
	DTO_INIT(DetailJsonVO, JsonVO<SampleDTO::Wrapper>);
};

/**
 * 示例分页显示JsonVO，用于响应给客户端的Json对象
 */
class SamplePageJsonVO : public JsonVO<SamplePageDTO::Wrapper> {
	DTO_INIT(SamplePageJsonVO, JsonVO<SamplePageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SAMPLE_VO_