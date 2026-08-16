#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28
*/
#ifndef _SUPPLY_DELIVERY_DAO_
#define _SUPPLY_DELIVERY_DAO_

#include "BaseDAO.h"

struct SupplyDeliverySummaryRow
{
	std::string requestId;
	std::string receiverId;
	std::string statusEnum;
	std::string receiveTime;
};

class SupplyDeliveryDAO : public BaseDAO
{
public:
	std::list<SupplyDeliverySummaryRow> selectSummaryByRequestIds(
		const std::vector<std::string>& requestIds);
};

#endif // !_SUPPLY_DELIVERY_DAO_
