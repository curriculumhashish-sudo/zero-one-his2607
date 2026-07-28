#pragma once
#ifndef _INOUTBOUNDMAPPER_H_
#define _INOUTBOUNDMAPPER_H_

#include "../../domain/do/data_statistics/InOutboundDo.h"

class InOutboundMapper : public Mapper<InOutboundDO> {
public:
	InOutboundDO mapper(ResultSet* resultSet) const override {
		InOutboundDO inOutboundDO;
		inOutboundDO.setName(resultSet->getString("name"));
		inOutboundDO.setBusNo(resultSet->getString("bus_no"));
		inOutboundDO.setLotNumberCostAmount(resultSet->getString("lot_number_cost_amount"));
		inOutboundDO.setLotNumberPriceAmount(resultSet->getString("lot_number_price_amount"));
		inOutboundDO.setLineCostTotal(resultSet->getString("line_cost_total"));
		inOutboundDO.setLinePriceTotal(resultSet->getString("line_price_total"));
		return inOutboundDO;
	}
};
#endif // !_INOUTBOUNDMAPPER_H_
