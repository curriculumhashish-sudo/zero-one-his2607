#pragma once

#ifndef _GET_CATEGORY_DETAILS_VO_
#define _GET_CATEGORY_DETAILS_VO_

#include "../../GlobalInclude.h"
#include "../../dto/settle/GetCategoryDetailsDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetCategoryDetailsJsonVO : public JsonVO<oatpp::Vector<GetCategoryDetailsDTO::Wrapper>> {
	DTO_INIT(GetCategoryDetailsJsonVO, JsonVO<oatpp::Vector<GetCategoryDetailsDTO::Wrapper>>);
};

#include OATPP_CODEGEN_END(DTO)
#endif