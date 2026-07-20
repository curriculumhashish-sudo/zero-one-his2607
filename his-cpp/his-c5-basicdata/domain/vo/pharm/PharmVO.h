#pragma once

#ifndef _PHARM_VO_
#define _PHARM_VO_

#include "../../GlobalInclude.h"
#include "../../dto/pharm/PharmDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PharmJsonVO : public JsonVO<PharmDTO::Wrapper> {
	DTO_INIT(PharmJsonVO, JsonVO<PharmDTO::Wrapper>);
};

class PharmPageJsonVO : public JsonVO<PharmPageDTO::Wrapper> {
	DTO_INIT(PharmPageJsonVO, JsonVO<PharmPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PHARM_VO_