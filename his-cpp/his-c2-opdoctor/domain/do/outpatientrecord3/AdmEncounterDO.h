#pragma once
#ifndef _AdmEncounterDO_H_
#define _AdmEncounterDO_H_

#include "../DoInclude.h"

/**
 * Database entity for adm_encounter table.
 * Lightweight DO: only maps fields needed by outpatientrecord3Service
 * (patient_id for derivation, status_enum for editability check).
 */
class AdmEncounterDO : public BaseDO
{
	// Primary key
	MYSQL_SYNTHESIZE(string, id, Id);
	// Patient ID, NOT NULL in schema
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// Status enum for editability gating
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

public:
	AdmEncounterDO() : BaseDO("adm_encounter")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
	}
};

typedef std::shared_ptr<AdmEncounterDO> PtrAdmEncounterDO;

#endif // !_AdmEncounterDO_H_
