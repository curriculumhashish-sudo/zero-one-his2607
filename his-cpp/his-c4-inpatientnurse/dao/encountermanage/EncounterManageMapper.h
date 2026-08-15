#pragma once
#ifndef _ENCOUNTER_MANAGE_MAPPER_H_
#define _ENCOUNTER_MANAGE_MAPPER_H_

#include "Mapper.h"
#include "domain/dto/encountermanage/PagePatientQueueDTO.h"

class PatientQueueMapper : public Mapper<PatientQueueItemDTO::Wrapper>
{
public:
	PatientQueueItemDTO::Wrapper mapper(ResultSet* rs) const override
	{
		auto dto = PatientQueueItemDTO::createShared();
		dto->accountId = rs->getString("account_id").asStdString();
		dto->accountBalance = rs->getDouble("account_balance");
		dto->availableBalance = rs->getDouble("available_balance");
		dto->prepayNetBalance = nullptr;
		dto->pendingSettlementTotal = rs->getDouble("pending_settlement_total");
		dto->totalAmount = rs->getDouble("settled_total") + dto->pendingSettlementTotal.getValue(0);
		dto->paidAmount = nullptr;
		dto->arrearsAmount = dto->availableBalance < 0 ? -dto->availableBalance : 0;
		dto->lowBalanceWarning = dto->availableBalance < 0;
		dto->accountBalanceDisplayMode = "BALANCE";
		dto->accountBalanceSubText = nullptr;
		dto->accountBalanceSummaryText = nullptr;
		dto->encounterId = rs->getString("encounter_id").asStdString();
		dto->encounterBusNo = rs->getString("encounter_bus_no").asStdString();
		dto->patientId = rs->getString("patient_id").asStdString();
		dto->patientName = rs->getString("patient_name").asStdString();
		dto->genderEnum = rs->getString("gender_enum").asStdString();
		dto->genderEnumText = dto->genderEnum == "FEMALE" ? ZH_WORDS_GETTER("business-text.enum.gender-female") :
			(dto->genderEnum == "MALE" ? ZH_WORDS_GETTER("business-text.enum.gender-male") : ZH_WORDS_GETTER("business-text.enum.unknown"));
		dto->ageText = rs->getString("age_text").asStdString();
		dto->pinyinCode = rs->getString("pinyin_code").asStdString();
		dto->wubiCode = rs->getString("wubi_code").asStdString();
		dto->statusEnum = rs->getString("status_enum").asStdString();
		if (dto->statusEnum == "REGISTERED")
			dto->statusEnumText = ZH_WORDS_GETTER("business-text.enum.encounter-registered");
		else if (dto->statusEnum == "PENDING")
			dto->statusEnumText = ZH_WORDS_GETTER("business-text.enum.encounter-pending-discharge");
		else if (dto->statusEnum == "COMPLETED")
			dto->statusEnumText = ZH_WORDS_GETTER("business-text.enum.encounter-settled");
		else if (dto->statusEnum == "DISCHARGED")
			dto->statusEnumText = ZH_WORDS_GETTER("business-text.enum.encounter-discharged");
		else
			dto->statusEnumText = ZH_WORDS_GETTER("business-text.enum.encounter-in-hospital");
		dto->priorityEnum = rs->getString("priority_enum").asStdString();
		dto->startTime = rs->getString("start_time").asStdString();
		dto->admissionTime = dto->startTime;
		dto->endTime = rs->getString("end_time").asStdString();
		dto->organizationId = rs->getString("organization_id").asStdString();
		dto->organizationName = rs->getString("organization_name").asStdString();
		dto->bedId = rs->getString("bed_id").asStdString();
		dto->bedName = rs->getString("bed_name").asStdString();
		dto->wardId = rs->getString("ward_id").asStdString();
		dto->wardName = rs->getString("ward_name").asStdString();
		dto->houseName = rs->getString("house_name").asStdString();
		dto->houseId = rs->getString("house_id").asStdString();
		dto->diagnosisSummary = rs->getString("diagnosis_summary").asStdString();
		dto->diagnosisList = oatpp::List<oatpp::String>::createShared();
		if (!dto->diagnosisSummary->empty()) dto->diagnosisList->push_back(dto->diagnosisSummary);
		dto->attendingDoctorName = rs->getString("doctor_name").asStdString();
		dto->primaryNurseName = rs->getString("nurse_name").asStdString();
		dto->pendingAuditCount = rs->getInt("pending_audit_count");
		dto->pendingExecuteCount = rs->getInt("pending_execute_count");
		dto->recallEnabled = dto->statusEnum == "PENDING" || dto->statusEnum == "COMPLETED"
			|| dto->statusEnum == "DISCHARGED";
		return dto;
	}
};

class WardNodeMapper : public Mapper<WardNodeDTO::Wrapper>
{
public:
	WardNodeDTO::Wrapper mapper(ResultSet* rs) const override
	{
		auto dto = WardNodeDTO::createShared();
		dto->wardId = rs->getString("ward_id").asStdString();
		dto->wardName = rs->getString("ward_name").asStdString();
		dto->displayOrder = rs->getInt("display_order");
		dto->houseNodes = oatpp::List<HouseNodeDTO::Wrapper>::createShared();
		return dto;
	}
};

class HouseNodeMapper : public Mapper<HouseNodeDTO::Wrapper>
{
public:
	HouseNodeDTO::Wrapper mapper(ResultSet* rs) const override
	{
		auto dto = HouseNodeDTO::createShared();
		dto->houseId = rs->getString("house_id").asStdString();
		dto->houseName = rs->getString("house_name").asStdString();
		dto->displayOrder = rs->getInt("display_order");
		return dto;
	}
};

#endif
