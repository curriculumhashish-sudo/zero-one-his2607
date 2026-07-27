#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_DETAIL_DO_
#define _HISTORY_ORDER_DETAIL_DO_
#include "../DoInclude.h"

/**
 * 历史医嘱明细 DO
 * 对应右侧医嘱表格展示字段，来自多张医嘱表的 UNION 查询
 */
class HistoryOrderDetailDO : public BaseDO
{
    // 医嘱唯一ID
    MYSQL_SYNTHESIZE(string, orderId, OrderId);
    // 医嘱类型（药品/检验/检查/耗材等）
    MYSQL_SYNTHESIZE(string, orderType, OrderType);
    // 医嘱名称
    MYSQL_SYNTHESIZE(string, orderName, OrderName);
    // 开立总次数
    MYSQL_SYNTHESIZE(int, quantity, Quantity);
    // 已执行次数
    MYSQL_SYNTHESIZE(int, executedCount, ExecutedCount);
    // 样本类型（仅检验类）
    MYSQL_SYNTHESIZE(string, sampleType, SampleType);
    // 执行医院
    MYSQL_SYNTHESIZE(string, executeHospital, ExecuteHospital);
    // 开立时间
    MYSQL_SYNTHESIZE(string, createTime, CreateTime);
    // 对应诊断
    MYSQL_SYNTHESIZE(string, diagnosis, Diagnosis);
    // 开立医生姓名
    MYSQL_SYNTHESIZE(string, doctorName, DoctorName);
    // 是否皮试（仅药品）
    MYSQL_SYNTHESIZE(int, isSkinTest, IsSkinTest);   // 用int表示布尔（0/1）

public:
    HistoryOrderDetailDO() : BaseDO("med_medication_request")   // 主表选药品医嘱表，实际查询为UNION，此处仅作标识
    {
        MYSQL_ADD_FIELD_PK("order_id", "s", orderId);
        MYSQL_ADD_FIELD("order_type", "s", orderType);
        MYSQL_ADD_FIELD("order_name", "s", orderName);
        MYSQL_ADD_FIELD("quantity", "i", quantity);
        MYSQL_ADD_FIELD("executed_count", "i", executedCount);
        MYSQL_ADD_FIELD_NULLABLE("sample_type", "s", sampleType, false);
        MYSQL_ADD_FIELD("execute_hospital", "s", executeHospital);
        MYSQL_ADD_FIELD("create_time", "s", createTime);
        MYSQL_ADD_FIELD("diagnosis", "s", diagnosis);
        MYSQL_ADD_FIELD("doctor_name", "s", doctorName);
        MYSQL_ADD_FIELD_NULLABLE("is_skin_test", "i", isSkinTest, false);
    }
};

typedef std::shared_ptr<HistoryOrderDetailDO> PtrHistoryOrderDetailDO;

#endif // !_HISTORY_ORDER_DETAIL_DO_