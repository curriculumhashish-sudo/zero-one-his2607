#pragma once
#ifndef _INPATIENT_BILLING_SERVICE_H_
#define _INPATIENT_BILLING_SERVICE_H_
#include "domain/vo/inpatientbilling/InpatientBillingVO.h"
#include "domain/dto/inpatientbilling/InpatientBillingDTO.h"

class InpatientBillingService
{
public:
	// 查询自动计费配置列表
	ListAutoRollConfigsDTO::Wrapper listAutoRollConfigs(const ListAutoRollConfigsQuery::Wrapper& query);
	// 新增自动计费规则
	CreateAutoGenRuleResponseDTO::Wrapper createAutoGenRule(const CreateAutoGenRuleDTO::Wrapper& dto);
	// 修改自动计费规则（仅覆盖非空字段）
	UpdateAutoGenRuleResponseDTO::Wrapper updateAutoGenRule(const UpdateAutoGenRuleDTO::Wrapper& dto);
	// 软删除自动计费规则
	DeleteAutoGenRuleResponseDTO::Wrapper deleteAutoGenRule(const DeleteAutoGenRuleDTO::Wrapper& dto);
	// 启用自动计费规则
	ActivateAutoGenRuleResponseDTO::Wrapper activateAutoGenRule(const ActivateAutoGenRuleDTO::Wrapper& dto);
	// 停用自动计费规则
	ActivateAutoGenRuleResponseDTO::Wrapper suspendAutoGenRule(const ActivateAutoGenRuleDTO::Wrapper& dto);
};

#endif // !_INPATIENT_BILLING_SERVICE_H_
