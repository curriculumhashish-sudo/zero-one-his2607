#pragma once
#ifndef _ENTERORDERSMAPPER_H_
#define _ENTERORDERSMAPPER_H_

#include "Mapper.h"
#include "../../domain/do/enter-orders/EnterOrdersDO.h"

//EnterOrders
//enter-orders
//ENTERORDERS

/**
 * 示例表字段匹配映射
 */
class EnterOrdersQueryMapper : public Mapper<EnterOrdersQueryDO>
{
public:
	EnterOrdersQueryDO mapper(ResultSet* resultSet) const override
	{
		EnterOrdersQueryDO data;
		data.setId(resultSet->getString(1));
		data.setTenant_id(resultSet->getString(2));
		data.setEncounter_id(resultSet->getString(3));
		data.setPatient_id(resultSet->getString(4));
		data.setGroup_id(resultSet->getString(5));
		data.setStatus(resultSet->getString(6));
		data.setMedicine_type(resultSet->getString(7));
		data.setTherapy_type(resultSet->getString(8));
		data.setMedicine_name(resultSet->getString(9));
		data.setSpecification(resultSet->getString(10));
		data.setDose(resultSet->getString(11));
		data.setDose_unit(resultSet->getString(12));
		data.setMethod(resultSet->getString(13));
		data.setRate(resultSet->getString(14));
		data.setTotal_price(resultSet->getString(15));
		data.setDescription(resultSet->getString(16));
		data.setAuthored_by(resultSet->getString(17));
		data.setSkin_text(resultSet->getString(18));
		return data;
	}
};

/**
 * 示例表字段匹配映射-创建智能指针对象
 */
class PtrEnterOrdersQueryMapper : public Mapper<PtrEnterOrdersQueryDO>
{
public:
	PtrEnterOrdersQueryDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<EnterOrdersQueryDO>();
		data->setId(resultSet->getString(1));
		data->setTenant_id(resultSet->getString(2));
		data->setEncounter_id(resultSet->getString(3));
		data->setPatient_id(resultSet->getString(4));
		data->setGroup_id(resultSet->getString(5));
		data->setStatus(resultSet->getString(6));
		data->setMedicine_type(resultSet->getString(7));
		data->setTherapy_type(resultSet->getString(8));
		data->setMedicine_name(resultSet->getString(9));
		data->setSpecification(resultSet->getString(10));
		data->setDose(resultSet->getString(11));
		data->setDose_unit(resultSet->getString(12));
		data->setMethod(resultSet->getString(13));
		data->setRate(resultSet->getString(14));
		data->setTotal_price(resultSet->getString(15));
		data->setDescription(resultSet->getString(16));
		data->setAuthored_by(resultSet->getString(17));
		data->setSkin_text(resultSet->getString(18));
		return data;
	}
};











#endif // !