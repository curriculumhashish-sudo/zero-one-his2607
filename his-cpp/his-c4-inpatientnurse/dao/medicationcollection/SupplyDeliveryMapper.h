#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28
*/
#ifndef _SUPPLY_DELIVERY_MAPPER_
#define _SUPPLY_DELIVERY_MAPPER_

#include "Mapper.h"
#include "SupplyDeliveryDAO.h"

class SupplyDeliverySummaryMapper : public Mapper<SupplyDeliverySummaryRow>
{
public:
	SupplyDeliverySummaryRow mapper(ResultSet* resultSet) const override
	{
		SupplyDeliverySummaryRow row;
		row.requestId = resultSet->getString("request_id");
		row.receiverId = resultSet->getString("receiver_id");
		row.statusEnum = resultSet->getString("status_enum");
		row.receiveTime = resultSet->getString("receive_time");
		return row;
	}
};

#endif // !_SUPPLY_DELIVERY_MAPPER_
