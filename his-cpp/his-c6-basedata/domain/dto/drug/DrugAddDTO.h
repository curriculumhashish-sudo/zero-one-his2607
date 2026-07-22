#pragma once
#ifndef  _DRUGADDDTO_H_
#define  _DRUGADDDTO_H_
#include "DrugDetailDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class DrugAddDTO: public DrugDetailDTO
{
	DTO_INIT(DrugAddDTO, DrugDetailDTO);

	
};
#include OATPP_CODEGEN_END(DTO);
#endif // !
