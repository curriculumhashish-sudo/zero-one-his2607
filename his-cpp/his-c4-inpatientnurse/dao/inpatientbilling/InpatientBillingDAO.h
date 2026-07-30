#pragma once
#ifndef _INPATIENT_BILLING_DAO_H_
#define _INPATIENT_BILLING_DAO_H_

#include "BaseDAO.h"
#include "domain/do/inpatientbilling/AutoGenRuleDO.h"
#include "domain/do/chargedetails/ChargeItemDO.h"

class InpatientBillingDAO : public BaseDAO
{
public:
	// 按就诊查询自动计费规则列表，按优先级和ID排序
	std::list<AutoGenRuleDO> selectRulesByEncounterId(const std::string& encounterId);
	// 统计就诊下的自动计费规则数量（排除已删除）
	uint64_t countRulesByEncounterId(const std::string& encounterId);

    // 根据项目编码查询收费项目信息（补费时查询用）
    std::unique_ptr<ChargeItemDO> selectChargeItemByCode(const std::string& itemCode);

    // 插入计费记录
    int insertChargeItem(const ChargeItemDO& chargeItem);

    // 根据患者ID和就诊ID查询费用明细
    std::list<ChargeItemDO> selectChargeItemsByPatientAndEncounter(
        const std::string& patientId,
        const std::string& encounterId
    );

    // 根据患者ID分页查询费用明细
    std::list<ChargeItemDO> selectChargeItemsByPatientId(
        const std::string& patientId,
        int offset,
        int limit
    );

    // 统计患者费用记录数量
    uint64_t countChargeItemsByPatientId(const std::string& patientId);

    // 根据医嘱ID查询是否已补费
    std::unique_ptr<ChargeItemDO> selectChargeItemByServiceId(const std::string& serviceId);

    // 更新费用记录状态
    int updateChargeItemStatus(const std::string& id, const std::string& status);
};

#endif // !_INPATIENT_BILLING_DAO_H_
