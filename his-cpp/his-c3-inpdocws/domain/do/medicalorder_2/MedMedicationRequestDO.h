#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/22 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/

#ifndef _MEDMEDICATIONREQUESTDO_H_
#define _MEDMEDICATIONREQUESTDO_H

#include "../DoInclude.h"

//药物数据库对象，映射med_medication_request表
class MedMedicationRequestDO : public BaseDO
{
	//主键
	MYSQL_SYNTHESIZE(std::string, id, Id);
	//组id，用于合并拆分
	MYSQL_SYNTHESIZE(std::string, groupId, GroupId);
	//状态——停嘱
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	//停止原因码
	MYSQL_SYNTHESIZE(int, statusReason, StatusReason);
	//状态变更时间
	MYSQL_SYNTHESIZE(std::string, statusChangedTime, StatusChangedTime);
	//拆嘱时记录原组号
	MYSQL_SYNTHESIZE(std::string, basedOnTable, BasedOnTable);
	//追溯到的源头表
	MYSQL_SYNTHESIZE(std::string, basedOnId, BasedOnId);
	//长临类型
	MYSQL_SYNTHESIZE(std::string, therapyEnum, TherapyEnum);
	//停药时间
	MYSQL_SYNTHESIZE(std::string, effectiveDoseEnd, EffectiveDoseEnd);
	//患者id
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	//就诊id
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	//药品ID
	MYSQL_SYNTHESIZE(std::string, medicationId, MedicationId);
	//商业编号
	MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
	//诊断编号
	MYSQL_SYNTHESIZE(std::string, prescriptionNo, PrescriptionNo);
public:
	MedMedicationRequestDO() : BaseDO("med_medication_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("group_id", "s", groupId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("status_reason", "i", statusReason);
		MYSQL_ADD_FIELD("status_changed_time", "s", statusChangedTime);
		MYSQL_ADD_FIELD("based_on_table", "s", basedOnTable);
		MYSQL_ADD_FIELD("based_on_id", "s", basedOnId);
		MYSQL_ADD_FIELD("therapy_enum", "s", therapyEnum);
		MYSQL_ADD_FIELD("effective_dose_end", "s", effectiveDoseEnd);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("medication_id", "s", medicationId);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
	}
};
typedef std::shared_ptr<MedMedicationRequestDO> PtrMedMedicationRequestDO;


#endif