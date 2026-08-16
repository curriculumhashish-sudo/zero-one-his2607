#pragma once

#include "../DoInclude.h"

class EnrollListDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    MYSQL_SYNTHESIZE(string, encounterBusNo, EncounterBusNo);
    MYSQL_SYNTHESIZE(string, patientName, PatientName);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
    MYSQL_SYNTHESIZE(string, organizationName, OrganizationName);
    MYSQL_SYNTHESIZE(string, wardId, WardId);
    MYSQL_SYNTHESIZE(string, wardName, WardName);
    MYSQL_SYNTHESIZE(string, bedId, BedId);
    MYSQL_SYNTHESIZE(string, bedName, BedName);
    MYSQL_SYNTHESIZE(string, startTime, StartTime);

public:
    EnrollListDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD_PK("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("encounter_bus_no", "s", encounterBusNo);
        MYSQL_ADD_FIELD("patient_name", "s", patientName);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("organization_id", "s", organizationId);
        MYSQL_ADD_FIELD("organization_name", "s", organizationName);
        MYSQL_ADD_FIELD("ward_id", "s", wardId);
        MYSQL_ADD_FIELD("ward_name", "s", wardName);
        MYSQL_ADD_FIELD("bed_id", "s", bedId);
        MYSQL_ADD_FIELD("bed_name", "s", bedName);
        MYSQL_ADD_FIELD("start_time", "s", startTime);
    }
};