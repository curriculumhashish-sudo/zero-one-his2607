/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/28
*/
#include "stdafx.h"
#include "SupplyDeliveryDAO.h"
#include "SupplyDeliveryMapper.h"

std::list<SupplyDeliverySummaryRow> SupplyDeliveryDAO::selectSummaryByRequestIds(
	const std::vector<std::string>& requestIds)
{
	if (requestIds.empty())
	{
		return {};
	}

	std::string sql =
		"SELECT request_id, receiver_id, status_enum, receive_time "
		"FROM wkf_supply_delivery "
		"WHERE is_deleted=0 AND request_id IN (";
	SqlParams params;
	for (size_t index = 0; index < requestIds.size(); ++index)
	{
		if (index > 0)
		{
			sql += ",";
		}
		sql += "?";
		SQLPARAMS_PUSH(params, "s", std::string, requestIds[index]);
	}
	sql += ") ORDER BY request_id ASC, receive_time DESC, id ASC";

	return sqlSession->executeQuery<SupplyDeliverySummaryRow>(
		sql, SupplyDeliverySummaryMapper(), params);
}
