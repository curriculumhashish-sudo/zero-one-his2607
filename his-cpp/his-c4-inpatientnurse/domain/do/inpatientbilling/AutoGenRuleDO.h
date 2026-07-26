#pragma once
#ifndef _INPATIENT_CHARGE_RECORD_DO_
#define _INPATIENT_CHARGE_RECORD_DO_

#include "../DoInclude.h"

/**
 * 住院计费记录 DO
 * 对应表：inpatient_charge_record
 */
class InpatientChargeRecordDO : public BaseDO
{
    // ===== 主键 =====
    MYSQL_SYNTHESIZE(long long, id, Id);
    MYSQL_SYNTHESIZE(string, chargeNo, ChargeNo); //计费流水线

    // ===== 患者信息 =====
    MYSQL_SYNTHESIZE(string, inpNo, InpNo); //患者住院号

    // ===== 医嘱信息 =====
    MYSQL_SYNTHESIZE(string, orderId, OrderId); //医嘱id

    // ===== 项目信息 =====
    MYSQL_SYNTHESIZE(string, itemCode, ItemCode); //收费项目编码
    MYSQL_SYNTHESIZE(string, itemName, ItemName); //收费项目名称
    MYSQL_SYNTHESIZE(string, spec, Spec); //项目规格
    MYSQL_SYNTHESIZE(string, unit, Unit); //计量单位

    // ===== 费用信息 ===== 
    MYSQL_SYNTHESIZE(double, price, Price); //单价
    MYSQL_SYNTHESIZE(int, quantity, Quantity); //数量
    MYSQL_SYNTHESIZE(double, subtotal, Subtotal); //总金额

    // ===== 费用分类 =====
    MYSQL_SYNTHESIZE(string, chargeType, ChargeType); //费用类型，区分药品/诊疗...

    // ===== 操作信息 =====
    MYSQL_SYNTHESIZE(string, operatorId, Operator); //操作人
    MYSQL_SYNTHESIZE(string, chargeTime, ChargeTime); //补费记账时间
    MYSQL_SYNTHESIZE(string, chargeDept, ChargeDept); //记账科室

    // ===== 来源标识 =====
    MYSQL_SYNTHESIZE(string, chargeSource, ChargeSource); //计费来源

    // ===== 补费原因 =====
    MYSQL_SYNTHESIZE(string, supplementReason, SupplementReason);

    // ===== 状态 =====
    MYSQL_SYNTHESIZE(string, status, Status); //费用状态

public:
    InpatientChargeRecordDO() : BaseDO("inpatient_charge_record")
    {
        MYSQL_ADD_FIELD_PK("id", "ll", id);
        MYSQL_ADD_FIELD("charge_no", "s", chargeNo);
        MYSQL_ADD_FIELD("inp_no", "s", inpNo);
        MYSQL_ADD_FIELD("order_id", "s", orderId);
        MYSQL_ADD_FIELD("item_code", "s", itemCode);
        MYSQL_ADD_FIELD("item_name", "s", itemName);
        MYSQL_ADD_FIELD("spec", "s", spec);
        MYSQL_ADD_FIELD("unit", "s", unit);
        MYSQL_ADD_FIELD("price", "d", price);
        MYSQL_ADD_FIELD("quantity", "i", quantity);
        MYSQL_ADD_FIELD("subtotal", "d", subtotal);
        MYSQL_ADD_FIELD("charge_type", "s", chargeType);
        MYSQL_ADD_FIELD("operator", "s", operatorId);
        MYSQL_ADD_FIELD("charge_time", "s", chargeTime);
        MYSQL_ADD_FIELD("charge_dept", "s", chargeDept);
        MYSQL_ADD_FIELD("charge_source", "s", chargeSource);
        MYSQL_ADD_FIELD("supplement_reason", "s", supplementReason);
        MYSQL_ADD_FIELD("status", "s", status);
    }
};

typedef std::shared_ptr<InpatientChargeRecordDO> PtrInpatientChargeRecordDO;

#endif // !_INPATIENT_CHARGE_RECORD_DO_