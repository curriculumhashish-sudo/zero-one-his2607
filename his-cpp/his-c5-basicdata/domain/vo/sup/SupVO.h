#pragma once

#ifndef _SUP_VO_
#define _SUP_VO_

#include "../../GlobalInclude.h"
#include "../../dto/sup/SupDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SupNameJsonVO : public JsonVO<SupNameDTO::Wrapper> {
	DTO_INIT(SupNameJsonVO, JsonVO<SupNameDTO::Wrapper>);
};



class SupJsonVO : public JsonVO<SupDTO::Wrapper> {
	DTO_INIT(SupJsonVO, JsonVO<SupDTO::Wrapper>);
};

class SupPageJsonVO : public JsonVO<SupPageDTO::Wrapper> {
	DTO_INIT(SupPageJsonVO, JsonVO<SupPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SUP_VO_