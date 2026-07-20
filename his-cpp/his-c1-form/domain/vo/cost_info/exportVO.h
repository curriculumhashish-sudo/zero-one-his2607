#pragma once

#ifndef _COSTINFO_EXPORT_VO_
#define _COSTINFO_EXPORT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/cost_info/queryCostInfoDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  测试：显示JsonVO，用于响应给客户端的Json对象
 */
class ExportVO : public JsonVO<CostInfoQuery::Wrapper> {
	DTO_INIT(ExportVO, JsonVO<CostInfoQuery::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SAMPLE_VO_