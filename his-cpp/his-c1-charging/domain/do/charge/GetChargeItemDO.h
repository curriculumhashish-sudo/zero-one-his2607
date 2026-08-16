#pragma once

#ifndef _GET_CHARGE_ITEM_DO_H_
#define _GET_CHARGE_ITEM_DO_H_

#include "../DoInclude.h"

/**
 * 收费项明细数据对象（接口2）
 * 对应 adm_charge_item 多表 JOIN 查询结果集
 * BaseDO("") 表示只读 DO，不绑定单表
 */
class GetChargeItemDO : public BaseDO
{
    // 收费项主键
    MYSQL_SYNTHESIZE(string, chargeItemId, ChargeItemId);
    // 来源业务表名
    MYSQL_SYNTHESIZE(string, serviceTable, ServiceTable);
    // 来源业务主键
    MYSQL_SYNTHESIZE(string, serviceId, ServiceId);
    // 收费项目展示名称（definition.charge_name）
    MYSQL_SYNTHESIZE(string, itemName, ItemName);
    // 收费明细业务流水号
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    // 单价
    MYSQL_SYNTHESIZE(string, unitPrice, UnitPrice);
    // 数量值
    MYSQL_SYNTHESIZE(string, quantityValue, QuantityValue);
    // 数量单位 code
    MYSQL_SYNTHESIZE(string, quantityUnit, QuantityUnit);
    // 费用分类编码（产品读模型.category_code 或 definition.type_code）
    MYSQL_SYNTHESIZE(string, chargeCategoryCode, ChargeCategoryCode);
    // 收费状态编码
    MYSQL_SYNTHESIZE(string, statusCode, StatusCode);
    // 行总价
    MYSQL_SYNTHESIZE(string, totalPrice, TotalPrice);
    // 收款人用户ID（performer_id）
    MYSQL_SYNTHESIZE(string, cashierUserId, CashierUserId);
    // 收款人姓名（JOIN adm_practitioner）
    MYSQL_SYNTHESIZE(string, cashierName, CashierName);
    // 记账账户主键
    MYSQL_SYNTHESIZE(string, accountId, AccountId);
    // 费用性质合同名称（via account -> contract_no）
    MYSQL_SYNTHESIZE(string, contractName, ContractName);
    // 费用性质合同主键
    MYSQL_SYNTHESIZE(string, contractId, ContractId);
    // 医疗类别（via adm_encounter_diagnosis）
    MYSQL_SYNTHESIZE(string, medTypeCode, MedTypeCode);
    // 医保目录编码（产品读模型.yb_no）
    MYSQL_SYNTHESIZE(string, ybNo, YbNo);
    // 就诊ID（WHERE条件用，不暴露给DTO）
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);

public:
    GetChargeItemDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD("charge_item_id", "s", chargeItemId);
        MYSQL_ADD_FIELD("service_table", "s", serviceTable);
        MYSQL_ADD_FIELD("service_id", "s", serviceId);
        MYSQL_ADD_FIELD("item_name", "s", itemName);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("unit_price", "s", unitPrice);
        MYSQL_ADD_FIELD("quantity_value", "s", quantityValue);
        MYSQL_ADD_FIELD("quantity_unit", "s", quantityUnit);
        MYSQL_ADD_FIELD("charge_category_code", "s", chargeCategoryCode);
        MYSQL_ADD_FIELD("status_code", "s", statusCode);
        MYSQL_ADD_FIELD("total_price", "s", totalPrice);
        MYSQL_ADD_FIELD("cashier_user_id", "s", cashierUserId);
        MYSQL_ADD_FIELD("cashier_name", "s", cashierName);
        MYSQL_ADD_FIELD("account_id", "s", accountId);
        MYSQL_ADD_FIELD("contract_name", "s", contractName);
        MYSQL_ADD_FIELD("contract_id", "s", contractId);
        MYSQL_ADD_FIELD("med_type_code", "s", medTypeCode);
        MYSQL_ADD_FIELD("yb_no", "s", ybNo);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
    }
};

typedef std::shared_ptr<GetChargeItemDO> PtrGetChargeItemDO;

#endif // !_GET_CHARGE_ITEM_DO_H_
