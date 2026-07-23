#pragma once
#ifndef _PAT_SELECT_PAGE_JSON_VO_H_
#define _PAT_SELECT_PAGE_JSON_VO_H_

#include "../../GlobalInclude.h"
#include "../../lib-oatpp/include/domain/vo/JsonVO.h"
#include "../../lib-oatpp/include/domain/dto/PageDTO.h"
#include "../../dto/pat/PatSelectDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 选择患者弹窗分页外层VO
 */
class PatSelectPageJsonVO : public JsonVO<PageDTO<PatSelectDTO::Wrapper>::Wrapper>
{
	DTO_INIT(PatSelectPageJsonVO, JsonVO<PageDTO<PatSelectDTO::Wrapper>::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_SELECT_PAGE_JSON_VO_H_