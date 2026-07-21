#pragma once

#ifndef _COSTINFO_VO_
#define _COSTINFO_VO_

#include "../../GlobalInclude.h"
#include "../../dto/cost_info/costInfoDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class CostInfoJsonVO : public JsonVO<CostInfoDTO::Wrapper> {
	DTO_INIT(CostInfoJsonVO, JsonVO<CostInfoDTO::Wrapper>);
};

class ExportVO : public JsonVO<CostInfoDTO::Wrapper> {
	DTO_INIT(ExportVO, JsonVO<CostInfoDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_COSTINFO_VO_