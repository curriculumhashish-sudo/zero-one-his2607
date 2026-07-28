#pragma once
#ifndef _InpatientBilling_DAO_H_
#define _InpatientBilling_DAO_H_

#include "BaseDAO.h"
#include "domain/do/inpatientbilling/AutoGenRuleDO.h"

class InpatientBillingDAO : public BaseDAO
{
public:
	// 按就诊查询自动计费规则列表，按优先级和ID排序
	std::list<AutoGenRuleDO> selectRulesByEncounterId(const std::string& encounterId);
	// 统计就诊下的自动计费规则数量（排除已删除）
	uint64_t countRulesByEncounterId(const std::string& encounterId);
};

#endif // !_InpatientBilling_DAO_H_
