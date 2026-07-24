#pragma once
#ifndef _BEDRESERVATION_DO_
#define _BEDRESERVATION_DO_
#include "../DoInclude.h"

class bkg_bed_reservation_extDO : public BaseDO
{
    // 分配id
    MYSQL_SYNTHESIZE(string, id, Id);
    // 预约id
    MYSQL_SYNTHESIZE(string, appointment_id, Appointment_id);
    // 床位
    MYSQL_SYNTHESIZE(string, location_id, Location_id);
    // 患者
    MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
    // 病区
    MYSQL_SYNTHESIZE(string, ward_location_id, Ward_location_id);
    // 入院日期
    MYSQL_SYNTHESIZE(string, intended_admission_date, Intended_admission_date);
    // 创建时间
    MYSQL_SYNTHESIZE(string, created_at, Created_at);
    // 创建人
    MYSQL_SYNTHESIZE(string, created_by, Created_by);
    // 是否删除
    MYSQL_SYNTHESIZE(bool, is_deleted, Is_deleted);
    CC_SYNTHESIZE(string, patient_name, Patient_name);

public:
    bkg_bed_reservation_extDO() : BaseDO("bkg_bed_reservation_ext")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("appointment_id", "s", appointment_id);
        MYSQL_ADD_FIELD("location_id", "s", location_id);
        MYSQL_ADD_FIELD("patient_id", "s", patient_id);
        MYSQL_ADD_FIELD("ward_location_id", "s", ward_location_id);
        MYSQL_ADD_FIELD("intended_admission_date", "s", intended_admission_date);
        MYSQL_ADD_FIELD("created_at", "s", created_at);
        MYSQL_ADD_FIELD("created_by", "s", created_by);
        MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
    }
};
typedef std::shared_ptr<bkg_bed_reservation_extDO> PtrBedReservationDO;
#endif
