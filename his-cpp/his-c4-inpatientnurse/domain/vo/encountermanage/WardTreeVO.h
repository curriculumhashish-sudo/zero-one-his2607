#pragma once
#ifndef _WARD_TREE_VO_H_
#define _WARD_TREE_VO_H_
#include "../../GlobalInclude.h"
#include "../../dto/encountermanage/WardTreeDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class WardTreeJsonVO : public JsonVO<WardTreeDTO::Wrapper>
{
	DTO_INIT(WardTreeJsonVO, JsonVO<WardTreeDTO::Wrapper>);
public:
	WardTreeJsonVO() { this->data = WardTreeDTO::createShared(); }
};
#include OATPP_CODEGEN_END(DTO)
#endif
