#pragma once

#ifndef _SERVICEREQUESTDO_H_
#define _SERVICEREQUESTDO_H_

#include "../DoInclude.h"

class ServiceRequestDO : public BaseDO
{
    // Service request identifier.
    MYSQL_SYNTHESIZE(std::string, id, Id);
    // Encounter identifier.
    MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
    // Optimistic lock version.
    MYSQL_SYNTHESIZE(int, version, Version);
    // Service request status.
    MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
    // Service request category.
    MYSQL_SYNTHESIZE(std::string, categoryEnum, CategoryEnum);
    // Completed print count.
    MYSQL_SYNTHESIZE(int, printCount, PrintCount);
    // Last update user.
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    // Last update time.
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);

public:
    // Bind fields to the wkf_service_request table.
    ServiceRequestDO() : BaseDO("wkf_service_request")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("version", "i", version);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
        MYSQL_ADD_FIELD("print_count", "i", printCount);
        MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
        MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
    }
};

typedef std::shared_ptr<ServiceRequestDO> PtrServiceRequestDO;

#endif // !_SERVICEREQUESTDO_H_
