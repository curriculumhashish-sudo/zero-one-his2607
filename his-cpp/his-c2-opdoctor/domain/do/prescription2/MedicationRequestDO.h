#pragma once

#ifndef _MEDICATIONREQUESTDO_H_
#define _MEDICATIONREQUESTDO_H_

#include "../DoInclude.h"

class MedicationRequestDO : public BaseDO
{
    // Medical advice identifier.
    MYSQL_SYNTHESIZE(std::string, id, Id);
    // Encounter identifier.
    MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
    // Optimistic lock version.
    MYSQL_SYNTHESIZE(int, version, Version);
    // Order group identifier.
    MYSQL_SYNTHESIZE(std::string, groupId, GroupId);
    // Medical advice status.
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    // Number of completed executions.
    MYSQL_SYNTHESIZE(int, executeNum, ExecuteNum);
    // Status change time.
    MYSQL_SYNTHESIZE(std::string, statusChangedTime, StatusChangedTime);
    // Signing practitioner identifier.
    MYSQL_SYNTHESIZE(std::string, performerCheckId, PerformerCheckId);
    // Signing time.
    MYSQL_SYNTHESIZE(std::string, checkTime, CheckTime);
    // Last update user.
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    // Last update time.
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);

public:
    // Bind fields to the med_medication_request table.
    MedicationRequestDO() : BaseDO("med_medication_request")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("version", "i", version);
        MYSQL_ADD_FIELD("group_id", "s", groupId);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("execute_num", "i", executeNum);
        MYSQL_ADD_FIELD("status_changed_time", "s", statusChangedTime);
        MYSQL_ADD_FIELD("performer_check_id", "s", performerCheckId);
        MYSQL_ADD_FIELD("check_time", "s", checkTime);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
    }
};

typedef std::shared_ptr<MedicationRequestDO> PtrMedicationRequestDO;

#endif // !_MEDICATIONREQUESTDO_H_
