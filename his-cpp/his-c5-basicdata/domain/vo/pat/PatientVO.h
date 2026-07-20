#ifndef _PATIENT_VO_
#define _PATIENT_VO_

#include "../../GlobalInclude.h"
#include "../../dto/pat/PatientDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者单条返回VO
 */
class PatientJsonVO : public JsonVO<PatientDTO::Wrapper> {
	DTO_INIT(PatientJsonVO, JsonVO<PatientDTO::Wrapper>);
};

/**
 * 患者分页返回VO
 */
class PatientPageJsonVO : public JsonVO<PageDTO<PatientDTO::Wrapper>::Wrapper> {
	DTO_INIT(PatientPageJsonVO, JsonVO<PageDTO<PatientDTO::Wrapper>::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PATIENT_VO_