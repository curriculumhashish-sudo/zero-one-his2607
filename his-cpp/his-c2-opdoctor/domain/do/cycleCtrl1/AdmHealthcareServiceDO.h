#pragma once
#ifndef _ADM_HEALTHCARE_SERVICE_DO_H_
#define _ADM_HEALTHCARE_SERVICE_DO_H_
#include "../DoInclude.h"

class AdmHealthcareServiceDO : public BaseDO
{
public:
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(string, name, Name);
    MYSQL_SYNTHESIZE(string, typeCode, TypeCode);
    MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
    MYSQL_SYNTHESIZE(string, offeredOrgId, OfferedOrgId);
    MYSQL_SYNTHESIZE(string, specialtyCode, SpecialtyCode);
    MYSQL_SYNTHESIZE(string, locationId, LocationId);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(int, activeFlag, ActiveFlag);
    MYSQL_SYNTHESIZE(int, appointmentRequiredFlag, AppointmentRequiredFlag);
    MYSQL_SYNTHESIZE(string, ybNo, YbNo);
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, comment, Comment);
    MYSQL_SYNTHESIZE(string, extraDetails, ExtraDetails);
    MYSQL_SYNTHESIZE(string, contact, Contact);
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    MYSQL_SYNTHESIZE(int, version, Version);

public:
    AdmHealthcareServiceDO() : BaseDO("adm_healthcare_service")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("type_code", "s", typeCode);
        MYSQL_ADD_FIELD("category_code", "s", categoryCode);
        MYSQL_ADD_FIELD("offered_org_id", "s", offeredOrgId);
        MYSQL_ADD_FIELD("specialty_code", "s", specialtyCode);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("active_flag", "i", activeFlag);
        MYSQL_ADD_FIELD("appointment_required_flag", "i", appointmentRequiredFlag);
        MYSQL_ADD_FIELD("yb_no", "s", ybNo);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("comment", "s", comment);
        MYSQL_ADD_FIELD("extra_details", "s", extraDetails);
        MYSQL_ADD_FIELD("contact", "s", contact);
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

typedef std::shared_ptr<AdmHealthcareServiceDO> PtrAdmHealthcareServiceDO;

#endif