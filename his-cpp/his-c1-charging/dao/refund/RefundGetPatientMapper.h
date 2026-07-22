// ============================================================================
// GetPatientMapper.h — 退费患者队列 · 结果集映射器
// ============================================================================
// Mapper 的工作流程：
//   1. DAO 执行 SQL → 获得 ResultSet（含 N 行）
//   2. DAO 调用 sqlSession->executeQuery<GetPatientDO>(sql, GetPatientMapper(), params)
//   3. 框架对 ResultSet 每行调用 mapper(resultSet) → 返回一个 GetPatientDO
//   4. 所有 DO 收集到 std::list<GetPatientDO> 返回给 DAO
// ============================================================================

#pragma once
#include "Mapper.h"
#include "../../domain/do/refund/GetPatientDO.h"

// 值语义 Mapper — 用于列表查询（返回 std::list<GetPatientDO>）
class GetPatientMapper : public Mapper<GetPatientDO>
{
public:
    GetPatientDO mapper(ResultSet* resultSet) const override
    {
        GetPatientDO data;

        // resultSet->getString("列别名") 从当前行取出指定列的值
        // 列别名必须与 SQL 中 AS xxx 完全一致，区分大小写
        data.setEncounterId(resultSet->getString("encounter_id"));
        data.setEncounterBusNo(resultSet->getString("encounter_bus_no"));
        data.setPatientId(resultSet->getString("patient_id"));
        data.setPatientName(resultSet->getString("patient_name"));
        data.setGenderCode(resultSet->getString("gender_code"));
        data.setGenderEnumText(resultSet->getString("gender_enum_text"));
        data.setAgeText(resultSet->getString("age_text"));
        data.setQueueLabelCode(resultSet->getString("queue_label_code"));
        data.setChargeItemStatusSummary(resultSet->getString("charge_item_status_summary"));
        data.setOrganizationName(resultSet->getString("organization_name"));
        data.setDoctorName(resultSet->getString("doctor_name"));
        data.setStartTime(resultSet->getString("start_time"));
        data.setTotalAmount(resultSet->getString("total_amount"));
        data.setPaidAmount(resultSet->getString("paid_amount"));
        return data;
    }
};

// 指针语义 Mapper — 用于需要 shared_ptr 的场景
class PtrGetPatientMapper : public Mapper<PtrGetPatientDO>
{
public:
    PtrGetPatientDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<GetPatientDO>();
        data->setEncounterId(resultSet->getString("encounter_id"));
        data->setEncounterBusNo(resultSet->getString("encounter_bus_no"));
        data->setPatientId(resultSet->getString("patient_id"));
        data->setPatientName(resultSet->getString("patient_name"));
        data->setGenderCode(resultSet->getString("gender_code"));
        data->setGenderEnumText(resultSet->getString("gender_enum_text"));
        data->setAgeText(resultSet->getString("age_text"));
        data->setQueueLabelCode(resultSet->getString("queue_label_code"));
        data->setChargeItemStatusSummary(resultSet->getString("charge_item_status_summary"));
        data->setOrganizationName(resultSet->getString("organization_name"));
        data->setDoctorName(resultSet->getString("doctor_name"));
        data->setStartTime(resultSet->getString("start_time"));
        data->setTotalAmount(resultSet->getString("total_amount"));
        data->setPaidAmount(resultSet->getString("paid_amount"));
        return data;
    }
};
