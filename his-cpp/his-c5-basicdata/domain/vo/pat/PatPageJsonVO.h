#pragma once

#ifndef _PAT_PAGE_JSON_VO_H_
#define _PAT_PAGE_JSON_VO_H_

#include "../../../../lib-oatpp/include/domain/vo/JsonVO.h"
#include "../../../../lib-oatpp/include/domain/dto/PageDTO.h"
#include "../../dto/pat/PatientPageItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者分页查询外层响应VO
 * Wraps the complete patient management page result.
 */
class PatPageJsonVO : public JsonVO<PageDTO<PatientPageItemDTO::Wrapper>::Wrapper>
{
	DTO_INIT(PatPageJsonVO, JsonVO<PageDTO<PatientPageItemDTO::Wrapper>::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_PAGE_JSON_VO_H_