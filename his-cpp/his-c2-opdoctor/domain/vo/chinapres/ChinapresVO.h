#pragma once

#ifndef _CHINAPRESVO_H_
#define _CHINAPRESVO_H_

#include"domain/dto/chinapres/ChinapresPageDTO.h"
#include"domain/dto/chinapres/ChinapresDetailDTO.h"
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresPageJsonVO : public  JsonVO<ChinapresPageDTO::Wrapper>
{
	DTO_INIT(ChinapresPageJsonVO, JsonVO<ChinapresDTO::Wrapper>);
};

class ChinapresDetailJsonVO : public  JsonVO<ChinapresDetailDTO::Wrapper>
{
	DTO_INIT(ChinapresDetailJsonVO, JsonVO<ChinapresDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CHINAPRESVO_H_