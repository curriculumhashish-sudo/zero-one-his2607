#pragma once
#ifndef _EPISODEOFCAREDO_
#define _EPISODEOFCAREDO_

#include "../DoInclude.h"

/**
 * adm_episode_of_care entity (care episode / treatment period)
 */
class EpisodeOfCareDO : public BaseDO
{
    // Primary key
    MYSQL_SYNTHESIZE(string, id, Id);
    // Tenant identifier
    MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);
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
    // Patient id
    MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
    // Organization / department id
    MYSQL_SYNTHESIZE(string, organization_id, Organization_id);
    // Business number
    MYSQL_SYNTHESIZE(string, bus_no, Bus_no);
    // Care team id
    MYSQL_SYNTHESIZE(string, care_team_id, Care_team_id);
    // Care type
    MYSQL_SYNTHESIZE(string, care_type, Care_type);
    // Care goal
    MYSQL_SYNTHESIZE(string, care_goal, Care_goal);
    // Status enum value
    MYSQL_SYNTHESIZE(string, status_enum, Status_enum);
    // Active flag
    MYSQL_SYNTHESIZE(bool, active_flag, Active_flag);
    // Start time
    MYSQL_SYNTHESIZE(string, start_time, Start_time);
    // End time
    MYSQL_SYNTHESIZE(string, end_time, End_time);

public:
    EpisodeOfCareDO() : BaseDO("adm_episode_of_care") {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
        MYSQL_ADD_FIELD("created_at", "s", created_at);
        MYSQL_ADD_FIELD("updated_at", "s", updated_at);
        MYSQL_ADD_FIELD("created_by", "s", created_by);
        MYSQL_ADD_FIELD("updated_by", "s", updated_by);
        MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
        MYSQL_ADD_FIELD("deleted_at", "s", deleted_at);
        MYSQL_ADD_FIELD("deleted_by", "s", deleted_by);
        MYSQL_ADD_FIELD("version", "i", version);
        MYSQL_ADD_FIELD("patient_id", "s", patient_id);
        MYSQL_ADD_FIELD("organization_id", "s", organization_id);
        MYSQL_ADD_FIELD("bus_no", "s", bus_no);
        MYSQL_ADD_FIELD("care_team_id", "s", care_team_id);
        MYSQL_ADD_FIELD("care_type", "s", care_type);
        MYSQL_ADD_FIELD("care_goal", "s", care_goal);
        MYSQL_ADD_FIELD("status_enum", "s", status_enum);
        MYSQL_ADD_FIELD("active_flag", "b", active_flag);
        MYSQL_ADD_FIELD("start_time", "s", start_time);
        MYSQL_ADD_FIELD("end_time", "s", end_time);
        // safe defaults for NOT NULL columns without service-provided values
        setIs_deleted(false);
        setVersion(0);
        setActive_flag(true);
    }
};

typedef std::shared_ptr<EpisodeOfCareDO> PtrEpisodeOfCareDO;

#endif
