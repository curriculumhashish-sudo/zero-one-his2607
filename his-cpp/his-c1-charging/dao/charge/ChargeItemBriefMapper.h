#pragma once

#ifndef _CHARGE_ITEM_BRIEF_MAPPER_H_
#define _CHARGE_ITEM_BRIEF_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/charge/ChargeItemBriefDO.h"

/**
 * ChargeItemBriefDO 的结果集映射器
 * 将收费项子查询的每一行映射为 ChargeItemBriefDO 实例
 * SQL 列别名必须与 getString() 的参数名严格一致
 */
class ChargeItemBriefMapper : public Mapper<ChargeItemBriefDO>
{
public:
    ChargeItemBriefDO mapper(ResultSet* resultSet) const override
    {
        ChargeItemBriefDO data;

        data.setChargeItemId(resultSet->getString("charge_item_id"));
        data.setStatusCode(resultSet->getString("status_code"));
        data.setTotalPrice(resultSet->getString("total_price"));
        data.setItemLabel(resultSet->getString("item_label"));
        data.setEncounterId(resultSet->getString("encounter_id"));

        return data;
    }
};

#endif // !_CHARGE_ITEM_BRIEF_MAPPER_H_
