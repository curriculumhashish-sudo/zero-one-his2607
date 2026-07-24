#ifndef _PHARMPAGEDTO_H_
#define _PHARMPAGEDTO_H_

#include "../../GlobalInclude.h"
#include "PharmDTO.h"
#include "domain/dto/PageDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class PharmPageDTO : public PageDTO<PharmDTO::Wrapper>
{
    DTO_INIT(PharmPageDTO, PageDTO<PharmDTO::Wrapper>)
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMPAGEDTO_H_