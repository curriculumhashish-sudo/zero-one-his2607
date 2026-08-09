#pragma once

#ifndef _ADMISSIONVO_H_
#define _ADMISSIONVO_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/doctor/Admission2ResultDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Admission2ResultJsonVO : public JsonVO<Admission2ResultDTO::Wrapper>
{
    DTO_INIT(Admission2ResultJsonVO, JsonVO<Admission2ResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
