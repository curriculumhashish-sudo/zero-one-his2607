#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_DETAIL_MAPPER_
#define _HISTORY_ORDER_DETAIL_MAPPER_

#include "Mapper.h"
#include "../../domain/do/prescription4/HistoryOrderDetailDO.h"

/**
 * 历史医嘱明细 Mapper（智能指针版本）
 * 映射字段与 HistoryOrderDetailDO 的 setter 对应
 */
class PtrHistoryOrderDetailMapper : public Mapper<PtrHistoryOrderDetailDO>
{
public:
    PtrHistoryOrderDetailDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<HistoryOrderDetailDO>();
        data->setOrderId(resultSet->getString("order_id"));
        data->setOrderType(resultSet->getString("order_type"));
        data->setOrderName(resultSet->getString("order_name"));
        data->setQuantity(resultSet->getInt("quantity"));
        data->setExecutedCount(resultSet->getInt("executed_count"));
        data->setSampleType(resultSet->getString("sample_type"));
        data->setExecuteHospital(resultSet->getString("execute_hospital"));
        data->setCreateTime(resultSet->getString("create_time"));
        data->setDiagnosis(resultSet->getString("diagnosis"));
        data->setDoctorName(resultSet->getString("doctor_name"));
        data->setIsSkinTest(resultSet->getInt("is_skin_test"));
        return data;
    }
};

#endif // !_HISTORY_ORDER_DETAIL_MAPPER_