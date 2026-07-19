#pragma once
#ifndef _QUIT_NUMBER_VO_
#define _QUIT_NUMBER_VO_
#include "../../GlobalInclude.h"
#include "../his-c1-charging/domain/dto/register/QuitNumberDTO.h"  

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 确认退号响应VO（继承JsonVO，封装退号信息）
 */
class QuitNumberJsonVO : public JsonVO<QuitNumberDTO::Wrapper> {
    DTO_INIT(QuitNumberJsonVO, JsonVO<QuitNumberDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_QUIT_NUMBER_VO_
