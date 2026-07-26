#pragma once
#ifndef _VITALSIGNDO_
#define _VITALSIGNDO_

#include "../DoInclude.h"

class VitalSignDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
    MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
    MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
    MYSQL_SYNTHESIZE(int, version, Version);

    MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
    MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
    MYSQL_SYNTHESIZE(std::string, recordedAt, RecordedAt);
    MYSQL_SYNTHESIZE(std::string, typeEnum, TypeEnum);
    MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
    MYSQL_SYNTHESIZE(std::string, locationId, LocationId);
    MYSQL_SYNTHESIZE(int, babyNo, BabyNo);
    MYSQL_SYNTHESIZE(std::string, recorderId, RecorderId);
    MYSQL_SYNTHESIZE(std::string, remark, Remark);

public:
    VitalSignDO() : BaseDO("dia_vital_sign")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
        MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
        MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
        MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
        MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
        // Business columns
        MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);
        MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
        MYSQL_ADD_FIELD_NULLABLE("recorded_at", "s", recordedAt, false);
        MYSQL_ADD_FIELD_NULLABLE("type_enum", "s", typeEnum, false);
        MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);
        MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
        MYSQL_ADD_FIELD_NULLABLE("baby_no", "i", babyNo, true);
        MYSQL_ADD_FIELD_NULLABLE("recorder_id", "s", recorderId, true);
        MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
    }
};

typedef std::shared_ptr<VitalSignDO> PtrVitalSignDO;

#endif
