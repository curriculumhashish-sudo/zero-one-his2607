#pragma once
#ifndef  _DrugSelectVO_H_
#define  _DrugSelectVO_H_
#include "../../GlobalInclude.h"
#include"../../dto/drug/DrugSelectDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class DrugSelectPageJsonVO : public JsonVO<DrugSelectPageDTO ::Wrapper>
{
    DTO_INIT(DrugSelectPageJsonVO,JsonVO<DrugSelectPageDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO);
#endif // !