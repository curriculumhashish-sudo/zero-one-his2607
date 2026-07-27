#ifndef _MEDICALORDERVERIFICATIONVO_H
#define _MEDICALORDERVERIFICATIONVO_H



#include "../../dto/medicalorderverification/medicalOrderVerificationDetailDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class medicalOrderVerificationJsonVO : public ListJsonVO<medicalOrderVerificationDetailDTO::Wrapper>
{
	DTO_INIT(medicalOrderVerificationJsonVO, ListJsonVO<medicalOrderVerificationDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif
