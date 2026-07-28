#ifndef _diagnose_JSON_VO_H_
#define _diagnose_JSON_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/Diagnose/DiagnoseDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class DiagnoseRemoveJsonVO : public JsonVO<DiagnoseRemoveDTO::Wrapper>
{
	DTO_INIT(DiagnoseRemoveJsonVO, JsonVO<DiagnoseRemoveDTO::Wrapper>);

};

#include OATPP_CODEGEN_END(DTO)

#endif 