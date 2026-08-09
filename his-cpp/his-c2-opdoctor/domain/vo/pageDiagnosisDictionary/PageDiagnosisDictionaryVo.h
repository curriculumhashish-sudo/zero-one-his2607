#pragma once
#ifndef PAGEDIAGNOSISDICTIONARYVO_H
#define PAGEDIAGNOSISDICTIONARYVO_H

#include "../../GlobalInclude.h"
#include "../../dto/pageDiagnosisDictionary/PageDiagnosisDictionaryDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PageDiagnosisDictionaryPageVo : public JsonVO<PageDiagnosisDictionaryPageDTO::Wrapper>
{
public:
	DTO_INIT(PageDiagnosisDictionaryPageVo, JsonVO<PageDiagnosisDictionaryPageDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)


#endif // !
