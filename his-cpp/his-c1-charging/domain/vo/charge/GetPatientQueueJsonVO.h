#pragma once
#ifndef _GET_PATIENT_QUEUE_VO_
#define _GET_PATIENT_QUEUE_VO_

#include "../../GlobalInclude.h"
#include"domain/dto/charge/GetPatientQueueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
	获取患者队列分页数据响应对象
*/
class GetPatientQueueJsonVO : public JsonVO<GetPatientQueuePageDTO::Wrapper>
{
	DTO_INIT(GetPatientQueueJsonVO, JsonVO<GetPatientQueuePageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif //!_GET_PATIENT_QUEUE_VO_