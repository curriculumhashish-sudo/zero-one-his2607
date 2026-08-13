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
		dto->patientName=dto->name;
		dto->content=rs->getString("content").asStdString();
		dto->type=rs->getString("therapy_enum").asStdString();
		dto->therapyEnum=dto->type;
		dto->statusEnum=rs->getString("status_enum").asStdString();
		dto->patientId=rs->getString("patient_id").asStdString();
		dto->encounterId=rs->getString("encounter_id").asStdString();
		dto->version=rs->getInt64("version");
		dto->listRowKey = dto->orderType.getValue("") + ":" + dto->requestId.getValue("");
		dto->statusDisplay = dto->statusEnum;
		dto->therapyDisplay = dto->therapyEnum;
		dto->adviceDisplayName=rs->getString("advice_display_name").asStdString();
		dto->specificationText=rs->getString("specification_text").asStdString();
		dto->dosageDisplay=rs->getString("dosage_display").asStdString();
		dto->usageDisplay=rs->getString("usage_display").asStdString();
		dto->frequencyDisplay=rs->getString("frequency_display").asStdString();
		dto->authoredTime=rs->getString("authored_time").asStdString();
		dto->stopTime=rs->getString("stop_time").asStdString();
		dto->requesterName=rs->getString("requester_name").asStdString();
		dto->performerLocationDisplay=rs->getString("performer_location_display").asStdString();
		dto->categoryEnum=rs->getString("medicine_category").asStdString();
		dto->groupNo=rs->getInt("group_no");
		dto->sortNumber=rs->getInt("sort_number");
		dto->tcmFlag=rs->getBoolean("tcm_flag");
		dto->prescriptionNo=rs->getString("prescription_no").asStdString();
		dto->lineRequestIds->push_back(dto->requestId);
		dto->sufferingFlag=rs->getBoolean("suffering_flag");
		dto->chineseHerbsDoseQuantity=rs->getDouble("chinese_herbs_dose_quantity");
		return dto;
	}
};
#endif
