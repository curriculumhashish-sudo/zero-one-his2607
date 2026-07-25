#pragma once

#ifndef _CHARGE_ITEM_MAPPER_H_
#define _CHARGE_ITEM_MAPPER_H_

#include "Mapper.h"
#include "domain/do/charge/ChargeItemDO.h"

/**
 * ChargeItemDO 结果集映射器
 * SQL 列别名必须与 getString() 参数名严格一致
 */
class ChargeItemMapper : public Mapper<ChargeItemDO>
{
public:
    ChargeItemDO mapper(ResultSet* resultSet) const override
    {
        ChargeItemDO data;
        data.setChargeItemId(resultSet->getString("charge_item_id"));
        data.setServiceTable(resultSet->getString("service_table"));
        data.setServiceId(resultSet->getString("service_id"));
        data.setItemName(resultSet->getString("item_name"));
        data.setBusNo(resultSet->getString("bus_no"));
        data.setUnitPrice(resultSet->getString("unit_price"));
        data.setQuantityValue(resultSet->getString("quantity_value"));
        data.setQuantityUnit(resultSet->getString("quantity_unit"));
        data.setChargeCategoryCode(resultSet->getString("charge_category_code"));
        data.setStatusCode(resultSet->getString("status_code"));
        data.setTotalPrice(resultSet->getString("total_price"));
        data.setCashierUserId(resultSet->getString("cashier_user_id"));
        data.setCashierName(resultSet->getString("cashier_name"));
        data.setAccountId(resultSet->getString("account_id"));
        data.setContractName(resultSet->getString("contract_name"));
        data.setContractId(resultSet->getString("contract_id"));
        data.setMedTypeCode(resultSet->getString("med_type_code"));
        data.setYbNo(resultSet->getString("yb_no"));
        data.setEncounterId(resultSet->getString("encounter_id"));
        return data;
    }
};

#endif // !_CHARGE_ITEM_MAPPER_H_
