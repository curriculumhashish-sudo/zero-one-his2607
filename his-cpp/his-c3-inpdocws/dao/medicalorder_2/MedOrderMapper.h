#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/23 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _MEDORDERMAPPER_H_
#define _MEDORDERMAPPER_H_
#include "Mapper.h"
#include "domain/do/medicalorder_2/MedMedicationRequestDO.h"

class MedOrderMapper : public Mapper<PtrMedMedicationRequestDO>
{
public:
	PtrMedMedicationRequestDO mapper(ResultSet* resultSet)const override
	{
		PtrMedMedicationRequestDO order = std::make_shared<MedMedicationRequestDO>();
		order->setId(resultSet->getString("id"));
		order->setGroupId(resultSet->getString("group_id"));
		order->setStatusEnum(resultSet->getString("status_enum"));
		order->setStatusReason(resultSet->getInt("status_reason"));
		order->setBasedOnTable(resultSet->getString("based_on_table"));
		order->setBasedOnId(resultSet->getString("based_on_id"));
		order->setTherapyEnum(resultSet->getString("therapy_enum"));
		order->setEffectiveDoseEnd(resultSet->getString("effective_dose_end"));
		order->setPatientId(resultSet->getString("patient_id"));
		order->setEncounterId(resultSet->getString("encounter_id"));
		order->setMedicationId(resultSet->getString("medication_id"));
		order->setBusNo(resultSet->getString("bus_no"));
		order->setPrescriptionNo(resultSet->getString("prescription_no"));
		order->setStatusChangedTime(resultSet->getString("status_changed_time"));
		return order;
	}
};




#endif