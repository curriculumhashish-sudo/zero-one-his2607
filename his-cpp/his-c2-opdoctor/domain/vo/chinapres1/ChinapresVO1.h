#pragma once

#ifndef _CHINAPRESVO_H_
#define _CHINAPRESVO_H_

#include"domain/dto/chinapres1/ChinapresPageDTO1.h"
#include"domain/dto/chinapres1/ChinapresDetailDTO1.h"
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresPageJsonVO1 : public  JsonVO<ChinapresPageDTO1::Wrapper>
{
	DTO_INIT(ChinapresPageJsonVO1, JsonVO<ChinapresDTO1::Wrapper>);
};

class ChinapresDetailJsonVO1 : public  JsonVO<ChinapresDetailDTO1::Wrapper>
{
	DTO_INIT(ChinapresDetailJsonVO1, JsonVO<ChinapresDetailDTO1::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CHINAPRESVO1_H_