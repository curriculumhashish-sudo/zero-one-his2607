#pragma once
#ifndef _BKG_SCHEDULE_TEMPLATE_DO_H_
#define _BKG_SCHEDULE_TEMPLATE_DO_H_
#include "../DoInclude.h"

class BkgScheduleTemplateDO : public BaseDO
{
public:
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
    MYSQL_SYNTHESIZE(string, templateCode, TemplateCode);
    MYSQL_SYNTHESIZE(string, templateName, TemplateName);
    MYSQL_SYNTHESIZE(string, scheduleMode, ScheduleMode);
    MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
    MYSQL_SYNTHESIZE(string, locationId, LocationId);
    MYSQL_SYNTHESIZE(string, healthcareServiceId, HealthcareServiceId);
    MYSQL_SYNTHESIZE(string, slotProvisionTemplateId, SlotProvisionTemplateId);
    MYSQL_SYNTHESIZE(string, defaultReleasePolicyId, DefaultReleasePolicyId);
    MYSQL_SYNTHESIZE(string, effectiveStart, EffectiveStart);
    MYSQL_SYNTHESIZE(string, effectiveEnd, EffectiveEnd);
    MYSQL_SYNTHESIZE(int, priority, Priority);
    MYSQL_SYNTHESIZE(string, sourcePractitionerRoleId, SourcePractitionerRoleId);
    MYSQL_SYNTHESIZE(int, enabled, Enabled);
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    MYSQL_SYNTHESIZE(int, version, Version);

public:
    BkgScheduleTemplateDO() : BaseDO("bkg_schedule_template")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("organization_id", "s", organizationId);
        MYSQL_ADD_FIELD("template_code", "s", templateCode);
        MYSQL_ADD_FIELD("template_name", "s", templateName);
        MYSQL_ADD_FIELD("schedule_mode", "s", scheduleMode);
        MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD("healthcare_service_id", "s", healthcareServiceId);
        MYSQL_ADD_FIELD("slot_provision_template_id", "s", slotProvisionTemplateId);
        MYSQL_ADD_FIELD("default_release_policy_id", "s", defaultReleasePolicyId);
        MYSQL_ADD_FIELD("effective_start", "s", effectiveStart);
        MYSQL_ADD_FIELD("effective_end", "s", effectiveEnd);
        MYSQL_ADD_FIELD("priority", "i", priority);
        MYSQL_ADD_FIELD("source_practitioner_role_id", "s", sourcePractitionerRoleId);
        MYSQL_ADD_FIELD("enabled", "i", enabled);
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

typedef std::shared_ptr<BkgScheduleTemplateDO> PtrBkgScheduleTemplateDO;

#endif