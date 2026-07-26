#pragma once

#ifndef _PRESCRIPTION3_DAO_H_
#define _PRESCRIPTION3_DAO_H_

#include "../../domain/do/prescription3/Prescription3OrderDO.h"
#include "BaseDAO.h"

#include <string>
#include <vector>

/**
 * Data access for medication, service, and device outpatient orders.
 */
class Prescription3DAO : public BaseDAO
{
public:
    std::vector<Prescription3OrderDO> selectByOrdersForUpdate(
        const std::vector<Prescription3OrderDO>& orders);

    std::vector<Prescription3OrderDO> selectByGroupIdsForUpdate(
        const std::vector<std::string>& groupIds);

    int softDelete(
        const std::vector<Prescription3OrderDO>& orders,
        const std::string& operatorId);

    int assignGroup(
        const std::vector<Prescription3OrderDO>& orders,
        const std::string& groupId,
        const std::string& operatorId);

    int clearGroups(
        const std::vector<Prescription3OrderDO>& orders,
        const std::string& operatorId);
};

#endif
