#ifndef _MEDICAL_ORDER_VERIFICATION_VO_H_
#define _MEDICAL_ORDER_VERIFICATION_VO_H_

#include "../../dto/medicalorderverification/MedicalOrderVerificationDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicalOrderVerificationJsonVO : public ListJsonVO<MedicalOrderVerificationDetailDTO::Wrapper>
{
	DTO_INIT(MedicalOrderVerificationJsonVO, ListJsonVO<MedicalOrderVerificationDetailDTO::Wrapper>);
};

class MedicalVerificationWorkbenchJsonVO : public JsonVO<MedicalVerificationWorkbenchDTO::Wrapper>
{
	DTO_INIT(MedicalVerificationWorkbenchJsonVO,
		JsonVO<MedicalVerificationWorkbenchDTO::Wrapper>);

public:
	MedicalVerificationWorkbenchJsonVO()
	{
		this->data = MedicalVerificationWorkbenchDTO::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICAL_ORDER_VERIFICATION_VO_H_
