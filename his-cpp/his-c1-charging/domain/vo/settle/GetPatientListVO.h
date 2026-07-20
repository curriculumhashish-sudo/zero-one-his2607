#pragma once

#ifndef _GET_PATIENT_LIST_VO_
#define _GET_PATIENT_LIST_VO_

#include "../../GlobalInclude.h"
#include "../../dto/settle/GetPatientListDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientListPageJsonVO : public JsonVO<GetPatientListPageDTO::Wrapper> {
	DTO_INIT(GetPatientListPageJsonVO, JsonVO<GetPatientListPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif