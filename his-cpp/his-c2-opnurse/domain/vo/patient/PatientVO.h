#ifndef _PATIENTVO_H_
#define _PATIENTVO_H_

#include "../../dto/patient/PatientAct.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  定义一个病人信息分页显示JsonVO对象，用于响应给客户端
 */
class PatientActPageJsonVO : public JsonVO<PatientActPageDTO::Wrapper>
{
	DTO_INIT(PatientActPageJsonVO, JsonVO<PatientActPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PATIENTVO_H_