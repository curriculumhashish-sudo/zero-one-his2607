#pragma once
#ifndef _WARD_LOCATION_MAPPER_H_
#define _WARD_LOCATION_MAPPER_H_

#include "Mapper.h"
#include "domain/do/ward/WardLocationDO.h"

class WardLocationMapper
    : public Mapper<PtrWardLocationDO>
{
public:
    PtrWardLocationDO mapper(
        ResultSet* resultSet
    ) const override
    {
        auto data =
            std::make_shared<WardLocationDO>();

        data->setId(
            resultSet->getString("id")
        );

        data->setTenantId(
            resultSet->getString("tenant_id")
        );

        data->setName(
            resultSet->getString("name")
        );

        data->setBusNo(
            resultSet->getString("bus_no")
        );

        data->setStatusEnum(
            resultSet->getString("status_enum")
        );

        data->setOperationalEnum(
            resultSet->getString(
                "operational_enum"
            )
        );

        data->setFormEnum(
            resultSet->getString("form_enum")
        );

        data->setModeEnum(
            resultSet->getString("mode_enum")
        );

        data->setOrganizationId(
            resultSet->getString(
                "organization_id"
            )
        );

        data->setDisplayOrder(
            resultSet->getInt("display_order")
        );

        data->setParentId(
            resultSet->getString("parent_id")
        );

        return data;
    }
};

#endif