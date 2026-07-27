#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/27
*/
#ifndef _HISTORY_ORDER_LIST_MAPPER_
#define _HISTORY_ORDER_LIST_MAPPER_

#include "Mapper.h"
#include "../../domain/do/prescription4/HistoryOrderListDO.h"

/**
 * 历史医嘱列表 Mapper（智能指针版本）
 * 映射字段与 HistoryOrderListDO 的 setter 对应
 */
class PtrHistoryOrderListMapper : public Mapper<PtrHistoryOrderListDO>
{
public:
    PtrHistoryOrderListDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<HistoryOrderListDO>();
        data->setVisitId(resultSet->getString("visit_id"));
        data->setVisitTime(resultSet->getString("visit_time"));
        data->setVisitType(resultSet->getString("visit_type"));
        data->setMainDiagnosis(resultSet->getString("main_diagnosis"));
        data->setDoctorName(resultSet->getString("doctor_name"));
        data->setDrugCount(resultSet->getInt("drug_count"));
        data->setExamCount(resultSet->getInt("exam_count"));
        data->setMaterialCount(resultSet->getInt("material_count"));
        return data;
    }
};

#endif // !_HISTORY_ORDER_LIST_MAPPER_