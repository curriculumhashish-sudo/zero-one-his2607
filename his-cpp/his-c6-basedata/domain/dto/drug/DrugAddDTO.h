#pragma once
#ifndef  _DRUGADDDTO_H_
#define  _DRUGADDDTO_H_
#include "DrugDetailDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class DrugAddDTO : public DrugDetailDTO
{
	DTO_INIT(DrugAddDTO, DrugDetailDTO);
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	// 杅擂苺桄
	std::string validate()
	{
	
		return "";

	};

};

#include OATPP_CODEGEN_END(DTO)
#endif // !
