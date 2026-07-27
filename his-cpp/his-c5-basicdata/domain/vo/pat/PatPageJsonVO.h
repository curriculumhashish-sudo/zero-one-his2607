#pragma once

#ifndef _PAT_PAGE_JSON_VO_H_
#define _PAT_PAGE_JSON_VO_H_

#include "../../../../lib-oatpp/include/domain/vo/JsonVO.h"
#include "../../../../lib-oatpp/include/domain/dto/PageDTO.h"
#include "../../dto/pat/PatDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者分页查询外层响应VO
 * data = 完整分页对象 PageDTO<PatDTO>
 */
class PatPageJsonVO : public JsonVO<PageDTO<PatDTO::Wrapper>::Wrapper>
{
	DTO_INIT(PatPageJsonVO, JsonVO<PageDTO<PatDTO::Wrapper>::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_PAGE_JSON_VO_H_