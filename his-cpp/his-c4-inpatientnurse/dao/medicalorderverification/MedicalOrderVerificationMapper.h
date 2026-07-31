#pragma once
#ifndef _MEDICAL_ORDER_VERIFICATION_MAPPER_H_
#define _MEDICAL_ORDER_VERIFICATION_MAPPER_H_
#include "Mapper.h"
#include "domain/dto/medicalorderverification/MedicalOrderVerificationDetailDTO.h"
class MedicalOrderVerificationMapper : public Mapper<MedicalOrderVerificationDetailDTO::Wrapper>
{
public:
	MedicalOrderVerificationDetailDTO::Wrapper mapper(ResultSet* rs) const override
	{
		auto dto=MedicalOrderVerificationDetailDTO::createShared();
		dto->requestId=rs->getString("request_id").asStdString(); dto->orderType=rs->getString("order_type").asStdString();
		dto->name=rs->getString("patient_name").asStdString(); dto->isPassed=rs->getString("is_passed").asStdString();
		dto->content=rs->getString("content").asStdString(); dto->type=rs->getString("medical_type").asStdString(); return dto;
	}
};
#endif
