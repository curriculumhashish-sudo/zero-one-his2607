#pragma once

#ifndef _GET_COST_DETAIL_MAPPER_H_
#define _GET_COST_DETAIL_MAPPER_H_

#include "Mapper.h"
#include "domain/do/settle/GetCostDetailDO.h"

// 患者住院费用汇总结果映射器
class GetCostDetailMapper : public Mapper<PtrGetCostDetailDO>
{
public:
	PtrGetCostDetailDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<GetCostDetailDO>();
		data->setPatientId(resultSet->getString("patient_id"));
		data->setPatientName(resultSet->getString("patient_name"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setEncounterBusNo(resultSet->getString("encounter_bus_no"));
		data->setTotalAmount(resultSet->getString("total_amount"));
		data->setSettledAmount(resultSet->getString("settled_amount"));
		data->setUnsettledAmount(resultSet->getString("unsettled_amount"));
		return data;
	}
};

// 患者住院费用明细结果映射器
class CostDetailItemMapper : public Mapper<PtrCostDetailItemDO>
{
public:
	PtrCostDetailItemDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<CostDetailItemDO>();
		data->setChargeItemId(resultSet->getString("charge_item_id"));
		data->setBusNo(resultSet->getString("bus_no"));
		data->setPrescriptionNo(resultSet->getString("prescription_no"));
		data->setItemName(resultSet->getString("item_name"));
		data->setChargeCategoryCode(resultSet->getString("charge_category_code"));
		data->setChargeCategoryName(resultSet->getString("charge_category_name"));
		data->setQuantityValue(resultSet->getString("quantity_value"));
		data->setQuantityUnit(resultSet->getString("quantity_unit"));
		data->setUnitPrice(resultSet->getString("unit_price"));
		data->setTotalPrice(resultSet->getString("total_price"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setOccurrenceTime(resultSet->getString("occurrence_time"));
		data->setPerformingOrgName(resultSet->getString("performing_org_name"));
		data->setEntererName(resultSet->getString("enterer_name"));
		return data;
	}
};

#endif
