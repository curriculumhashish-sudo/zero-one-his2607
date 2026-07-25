#pragma once
#ifndef _BKG_WAITLIST_ENTRY_DO_H_
#define _BKG_WAITLIST_ENTRY_DO_H_
#include "../DoInclude.h"

// 前向声明，避免在此头文件使用智能指针类型时未定义导致的编译错误
class AdmPatientDO;
typedef std::shared_ptr<AdmPatientDO> PtrAdmPatientDO;

class BkgScheduleTemplateDO;
typedef std::shared_ptr<BkgScheduleTemplateDO> PtrBkgScheduleTemplateDO;

class AdmHealthcareServiceDO;
typedef std::shared_ptr<AdmHealthcareServiceDO> PtrAdmHealthcareServiceDO;



class BkgWaitlistEntryDO : public BaseDO
{
public:
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    MYSQL_SYNTHESIZE(string, slotId, SlotId);
    MYSQL_SYNTHESIZE(string, scheduleId, ScheduleId);
    MYSQL_SYNTHESIZE(int, queuePosition, QueuePosition);
    MYSQL_SYNTHESIZE(string, status, Status);
    MYSQL_SYNTHESIZE(string, promotedAppointmentId, PromotedAppointmentId);
    MYSQL_SYNTHESIZE(string, cancelReason, CancelReason);
    MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
    MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
    MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
    MYSQL_SYNTHESIZE(int, version, Version);

    //关联其他表里的字段
	CC_SYNTHESIZE(PtrAdmPatientDO, patientDO, PatientDO);
	CC_SYNTHESIZE(PtrBkgScheduleTemplateDO, scheduleTemplateDO, ScheduleTemplateDO);
	CC_SYNTHESIZE(PtrAdmHealthcareServiceDO, healthcareServiceDO, HealthcareServiceDO);


public:
    BkgWaitlistEntryDO() : BaseDO("bkg_waitlist_entry")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
        MYSQL_ADD_FIELD("organization_id", "s", organizationId);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("slot_id", "s", slotId);
        MYSQL_ADD_FIELD("schedule_id", "s", scheduleId);
        MYSQL_ADD_FIELD("queue_position", "i", queuePosition);
        MYSQL_ADD_FIELD("status", "s", status);
        MYSQL_ADD_FIELD("promoted_appointment_id", "s", promotedAppointmentId);
        MYSQL_ADD_FIELD("cancel_reason", "s", cancelReason);
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

typedef std::shared_ptr<BkgWaitlistEntryDO> PtrBkgWaitlistEntryDO;


#endif