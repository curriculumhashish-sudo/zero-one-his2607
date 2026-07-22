#pragma once

#ifndef _CHINAPRESPAGEDTO_H_
#define _CHINAPRESPAGEDTO_H_

#include "../../GlobalInclude.h"
#include "ChinapresDTO1.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresPageDTO1: public PageDTO<ChinapresDTO1::Wrapper>
{
	DTO_INIT(ChinapresPageDTO1, PageDTO<ChinapresDTO1::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHINAPRESPAGEDTO_H_