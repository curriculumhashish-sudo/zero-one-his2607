#pragma once

#ifndef _CHINAPRESPAGEDTO_H_
#define _CHINAPRESPAGEDTO_H_

#include "../../GlobalInclude.h"
#include "ChinapresDTO.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapresPageDTO : public PageDTO<ChinapresDTO::Wrapper>
{
	DTO_INIT(ChinapresPageDTO, PageDTO<ChinapresDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHINAPRESPAGEDTO_H_