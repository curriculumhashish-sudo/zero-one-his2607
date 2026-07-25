#pragma once
#ifndef _PHARMDETAILVO_H_
#define _PHARMDETAILVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/pharm/PharmDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class PharmJsonVO : public JsonVO<PharmDTO::Wrapper>
{
    DTO_INIT(PharmJsonVO, JsonVO<PharmDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMDETAILVO_H_