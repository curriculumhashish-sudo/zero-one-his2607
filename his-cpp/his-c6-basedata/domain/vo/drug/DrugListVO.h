#pragma once
#ifndef  _DrugListVO_H_
#define  _DrugListVO_H_
#include "../../GlobalInclude.h"
#include"../../dto/drug/DrugListDTO.h"
#include "domain/dto/drug/DrugDetailDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class DrugListPageJsonVO : public JsonVO<DrugListPageDTO::Wrapper>
{
    DTO_INIT(DrugListPageJsonVO,JsonVO<DrugListPageDTO::Wrapper>);
};

//响应给 药品详情客户端对象 
class DrugDetailJsonVO : public JsonVO<DrugDetailDTO::Wrapper>
{
	DTO_INIT(DrugDetailJsonVO, JsonVO<DrugDetailDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO)
#endif // !
