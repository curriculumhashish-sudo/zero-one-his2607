#pragma once
#ifndef _ADM_PATIENT_DO_H_
#define _ADM_PATIENT_DO_H_
#include "../DoInclude.h"

class AdmPatientDO : public BaseDO
{
public:
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(string, name, Name);
    MYSQL_SYNTHESIZE(string, genderEnum, GenderEnum);
    MYSQL_SYNTHESIZE(string, birthDate, BirthDate);
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    MYSQL_SYNTHESIZE(int, version, Version);

public:
    AdmPatientDO() : BaseDO("adm_patient")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("gender_enum", "s", genderEnum);
        MYSQL_ADD_FIELD("birth_date", "s", birthDate);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("created_at", "s", createdAt);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
        MYSQL_ADD_FIELD("created_by", "s", createdBy);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
        MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
        MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
        MYSQL_ADD_FIELD("version", "i", version);
    }
};

typedef std::shared_ptr<AdmPatientDO> PtrAdmPatientDO;

#endif