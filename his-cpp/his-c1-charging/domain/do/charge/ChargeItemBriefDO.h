#pragma once

#ifndef _CHARGE_ITEM_BRIEF_DO_H_
#define _CHARGE_ITEM_BRIEF_DO_H_

#include "../DoInclude.h"

/**
 * 收费项摘要数据对象
 * 用于患者队列响应中的 cashierChargeItems[] 子列表
 * 子查询：adm_charge_item JOIN adm_charge_item_definition
 * BaseDO("") 表示这是一个只读 DO，不绑定单张数据表
 */
class ChargeItemBriefDO : public BaseDO
{
    // 收费项主键
    MYSQL_SYNTHESIZE(string, chargeItemId, ChargeItemId);
    // 收费状态编码（PENDING / SETTLED / REFUNDING...）
    MYSQL_SYNTHESIZE(string, statusCode, StatusCode);
    // 行总价
    MYSQL_SYNTHESIZE(string, totalPrice, TotalPrice);
    // 收费项目展示名称（来自 adm_charge_item_definition.charge_name）
    MYSQL_SYNTHESIZE(string, itemLabel, ItemLabel);
    // 就诊ID，用于按 encounterId 分组（不直接暴露给 DTO）
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);

public:
    ChargeItemBriefDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("charge_item_id", "s", chargeItemId);
        MYSQL_ADD_FIELD("status_code", "s", statusCode);
        MYSQL_ADD_FIELD("total_price", "s", totalPrice);
        MYSQL_ADD_FIELD("item_label", "s", itemLabel);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
    }
};

typedef std::shared_ptr<ChargeItemBriefDO> PtrChargeItemBriefDO;

#endif // !_CHARGE_ITEM_BRIEF_DO_H_
