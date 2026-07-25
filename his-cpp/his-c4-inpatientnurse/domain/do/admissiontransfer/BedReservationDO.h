#pragma once
#ifndef _BEDRESERVATION_DO_
#define _BEDRESERVATION_DO_
#include "../DoInclude.h"

class bkg_bed_reservation_extDO : public BaseDO
{
    // Primary key
    MYSQL_SYNTHESIZE(string, id, Id);
    // Tenant identifier
    MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);
    // Related appointment id
    MYSQL_SYNTHESIZE(string, appointment_id, Appointment_id);
    // Bed / room location id
    MYSQL_SYNTHESIZE(string, location_id, Location_id);
    // Patient id
    MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
    // Ward location id
    MYSQL_SYNTHESIZE(string, ward_location_id, Ward_location_id);
    // Intended admission date
    MYSQL_SYNTHESIZE(string, intended_admission_date, Intended_admission_date);
    // Remark
    MYSQL_SYNTHESIZE(string, remark, Remark);
    // Creation time
    MYSQL_SYNTHESIZE(string, created_at, Created_at);
    // Last update time
    MYSQL_SYNTHESIZE(string, updated_at, Updated_at);
    // Creator
    MYSQL_SYNTHESIZE(string, created_by, Created_by);
    // Last updater
    MYSQL_SYNTHESIZE(string, updated_by, Updated_by);
    // Soft-delete flag
    MYSQL_SYNTHESIZE(bool, is_deleted, Is_deleted);
    // Deletion time
    MYSQL_SYNTHESIZE(string, deleted_at, Deleted_at);
    // Deleter
    MYSQL_SYNTHESIZE(string, deleted_by, Deleted_by);
    // Optimistic lock version
    MYSQL_SYNTHESIZE(int, version, Version);
    // Patient name (memory only, not a DB column)
    CC_SYNTHESIZE(string, patient_name, Patient_name);

public:
    bkg_bed_reservation_extDO() : BaseDO("bkg_bed_reservation_ext")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
        MYSQL_ADD_FIELD("appointment_id", "s", appointment_id);
        MYSQL_ADD_FIELD("location_id", "s", location_id);
        MYSQL_ADD_FIELD("patient_id", "s", patient_id);
        MYSQL_ADD_FIELD("ward_location_id", "s", ward_location_id);
        MYSQL_ADD_FIELD("intended_admission_date", "s", intended_admission_date);
        MYSQL_ADD_FIELD("remark", "s", remark);
        MYSQL_ADD_FIELD("created_at", "s", created_at);
        MYSQL_ADD_FIELD("updated_at", "s", updated_at);
        MYSQL_ADD_FIELD("created_by", "s", created_by);
        MYSQL_ADD_FIELD("updated_by", "s", updated_by);
        MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
        MYSQL_ADD_FIELD("deleted_at", "s", deleted_at);
        MYSQL_ADD_FIELD("deleted_by", "s", deleted_by);
        MYSQL_ADD_FIELD("version", "i", version);
        // safe defaults for NOT NULL columns not set by the service
        setIs_deleted(false);
        setVersion(0);
    }
};
typedef std::shared_ptr<bkg_bed_reservation_extDO> PtrBedReservationDO;
#endif
