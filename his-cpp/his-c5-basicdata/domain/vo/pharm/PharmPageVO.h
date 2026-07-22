#ifndef _PHARMPAGEVO_H_
#define _PHARMPAGEVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/pharm/PharmPageDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class PharmPageJsonVO : public JsonVO<PharmPageDTO::Wrapper>
{
    DTO_INIT(PharmPageJsonVO, JsonVO<PharmPageDTO::Wrapper>)
public:
    PharmPageJsonVO()
    {
        data = {};
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMPAGEVO_H_