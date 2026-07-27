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
		dto->prepayNetBalance = rs->getDouble("account_balance");
		dto->pendingSettlementTotal = rs->getDouble("pending_settlement_total");
		dto->totalAmount = rs->getDouble("settled_total");
		dto->paidAmount = dto->accountBalance;
		dto->arrearsAmount = dto->availableBalance < 0 ? -dto->availableBalance : 0;
		dto->lowBalanceWarning = dto->availableBalance < 0;
		dto->accountBalanceDisplayMode = "BALANCE";
		dto->accountBalanceSubText = "可用余额";
		dto->accountBalanceSummaryText = "余额 " + std::to_string(dto->availableBalance.getValue(0)) + " 元";
		dto->encounterId = rs->getString("encounter_id").asStdString();
		dto->encounterBusNo = rs->getString("encounter_bus_no").asStdString();
		dto->patientId = rs->getString("patient_id").asStdString();
		dto->patientName = rs->getString("patient_name").asStdString();
		dto->genderEnum = rs->getString("gender_enum").asStdString();
		dto->genderEnumText = dto->genderEnum == "FEMALE" ? "女" : (dto->genderEnum == "MALE" ? "男" : "未知");
		dto->ageText = rs->getString("age_text").asStdString();
		dto->pinyinCode = rs->getString("pinyin_code").asStdString();
		dto->wubiCode = rs->getString("wubi_code").asStdString();
		dto->statusEnum = rs->getString("status_enum").asStdString();
		dto->statusEnumText = dto->statusEnum == "COMPLETED" ? "已出院" : (dto->statusEnum == "DISCHARGE_PROCESSING" ? "待出院" : "在院");
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
		dto->diagnosisSummary = rs->getString("diagnosis_summary").asStdString();
		dto->diagnosisList = oatpp::List<oatpp::String>::createShared();
		if (!dto->diagnosisSummary->empty()) dto->diagnosisList->push_back(dto->diagnosisSummary);
		dto->attendingDoctorName = rs->getString("doctor_name").asStdString();
		dto->primaryNurseName = rs->getString("nurse_name").asStdString();
		dto->pendingAuditCount = rs->getInt("pending_audit_count");
		dto->pendingExecuteCount = rs->getInt("pending_execute_count");
		dto->recallEnabled = dto->statusEnum == "COMPLETED";
		return dto;
	}
};

#endif
