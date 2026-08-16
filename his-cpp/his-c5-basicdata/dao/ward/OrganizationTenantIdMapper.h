#pragma once
#ifndef _ORGANIZATION_TENANT_ID_MAPPER_H_
#define _ORGANIZATION_TENANT_ID_MAPPER_H_

#include <string>
#include "Mapper.h"

class OrganizationTenantIdMapper
    : public Mapper<std::string>
{
public:
    std::string mapper(
        ResultSet* resultSet
    ) const override
    {
        return resultSet->getString("tenant_id");
    }
};

#endif