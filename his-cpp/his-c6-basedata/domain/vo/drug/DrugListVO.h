#pragma once
#ifndef  _DrugListVO_H_
#define  _DrugListVO_H_
#include "../../GlobalInclude.h"
#include"../../dto/drug/DrugListDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class DrugListPageJsonVO : public JsonVO<DrugListPageDTO::Wrapper>
{
    DTO_INIT(DrugListPageJsonVO,JsonVO<DrugListPageDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO);
#endif // !