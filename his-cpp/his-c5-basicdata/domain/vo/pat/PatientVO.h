#pragma once

#ifndef _PATIENT_VO_
#define _PATIENT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/pat/PatientDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PatientJsonVO : public JsonVO<PatientDTO::Wrapper> {
	DTO_INIT(PatientJsonVO, JsonVO<PatientDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_VO_
