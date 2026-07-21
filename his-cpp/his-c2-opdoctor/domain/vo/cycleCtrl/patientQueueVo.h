#pragma once
#ifndef _PATIENTQUEUEVO_H_
#define _PATIENTQUEUEVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/cycleCtrl/patientQueueDTO.h"



#include OATPP_CODEGEN_BEGIN(DTO)

//显示JsonVO，用于响应给客户端的JSON对象
class PatientQueueJsonVO : public JsonVO<PatientQueueDTO::Wrapper>
{
	DTO_INIT(PatientQueueJsonVO, JsonVO<PatientQueueDTO::Wrapper>);
};




#include OATPP_CODEGEN_END(DTO)


#endif // !_PATIENTQUEUEVO_H_
