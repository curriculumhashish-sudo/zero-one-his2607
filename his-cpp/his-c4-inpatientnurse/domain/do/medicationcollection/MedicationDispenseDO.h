#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
 @Date: 2026/07/18 22:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _MEDICATIONDISPENSE_DO_
#define _MEDICATIONDISPENSE_DO_
#include "../DoInclude.h"

/**
 * Database entity for table med_medication_dispense.
 * Core fields for the summary drug-collection (dispense) feature.
 * NOTE: the table has more columns; extend on demand.
 */
class MedicationDispenseDO : public BaseDO
{
	// primary key
	MYSQL_SYNTHESIZE(string, id, Id);
	// summary bill no (groups multiple dispense records into one collection bill)
	MYSQL_SYNTHESIZE(string, summaryNo, SummaryNo);
	// bill status enum (WAIT_REVIEW / DISPENSED)
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// dispense type enum
	MYSQL_SYNTHESIZE(string, dispenseEnum, DispenseEnum);
	// medication id (FK -> med_medication_definition.id)
	MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
	// patient id
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// encounter id
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// pharmacy location id
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	// destination location id (ward that collects the drugs)
	MYSQL_SYNTHESIZE(string, desLocationId, DesLocationId);
	// medication request id (FK -> med_medication_request.id)
	MYSQL_SYNTHESIZE(string, medReqId, MedReqId);
	// requested quantity
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	// unit code (specification)
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	// actually dispensed quantity
	MYSQL_SYNTHESIZE(double, dispenseQuantity, DispenseQuantity);
	// lot number
	MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
	// preparer id
	MYSQL_SYNTHESIZE(string, preparerId, PreparerId);
	// prepare time
	MYSQL_SYNTHESIZE(string, prepareTime, PrepareTime);
	// dispenser practitioner id
	MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
	// dispense time
	MYSQL_SYNTHESIZE(string, dispenseTime, DispenseTime);
	// planned dispense time
	MYSQL_SYNTHESIZE(string, plannedDispenseTime, PlannedDispenseTime);
	// receiver practitioner id (the nurse who collects)
	MYSQL_SYNTHESIZE(string, recPractitionerId, RecPractitionerId);
public:
	MedicationDispenseDO() : BaseDO("med_medication_dispense")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("summary_no", "s", summaryNo);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("dispense_enum", "s", dispenseEnum);
		MYSQL_ADD_FIELD("medication_id", "s", medicationId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("location_id", "s", locationId);
		MYSQL_ADD_FIELD("des_location_id", "s", desLocationId);
		MYSQL_ADD_FIELD("med_req_id", "s", medReqId);
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD("unit_code", "s", unitCode);
		MYSQL_ADD_FIELD("dispense_quantity", "d", dispenseQuantity);
		MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
		MYSQL_ADD_FIELD("preparer_id", "s", preparerId);
		MYSQL_ADD_FIELD("prepare_time", "s", prepareTime);
		MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
		MYSQL_ADD_FIELD("dispense_time", "s", dispenseTime);
		MYSQL_ADD_FIELD("planned_dispense_time", "s", plannedDispenseTime);
		MYSQL_ADD_FIELD("rec_practitioner_id", "s", recPractitionerId);
	}
};

// shared_ptr alias for MedicationDispenseDO
typedef std::shared_ptr<MedicationDispenseDO> PtrMedicationDispenseDO;
#endif // !_MEDICATIONDISPENSE_DO_
