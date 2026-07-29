#include "stdafx.h"
#include "InpatientBillingService.h"
#include "dao/inpatientbilling/InpatientBillingDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

ListAutoRollConfigsDTO::Wrapper InpatientBillingService::listAutoRollConfigs(const ListAutoRollConfigsQuery::Wrapper& query)
{
	auto result = ListAutoRollConfigsDTO::createShared();
	result->configs = oatpp::List<AutoRollConfigDTO::Wrapper>::createShared();
	result->nursingLevelReadonly = false;
	if (!query|| !query->encounterId || query->encounterId->empty()) return result;
	InpatientBillingDAO dao;
	auto rules = dao.selectRulesByEncounterId(query->encounterId.getValue(""));
	for (auto& rule : rules)
	{
		auto dto = AutoRollConfigDTO::createShared();
		dto->ruleId = rule.getId();
		dto->ruleName = rule.getRuleName();
		dto->activityId = rule.getTriggerSourceId();
		dto->activityName = "";
		dto->chargeDefinitionId = rule.getChargeDefinitionId();
		dto->quantityValue = rule.getQuantityValue();
		dto->isActive = (rule.getStatusEnum() == "ACTIVE");
		result->configs->push_back(dto);
	}
	return result;
}

CreateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::createAutoGenRule(const CreateAutoGenRuleDTO::Wrapper& dto)
{
	auto response = CreateAutoGenRuleResponseDTO::createShared();
	if (!dto) return response;
	AutoGenRuleDO data;
	UuidFacade uf;
	data.setId(uf.genUuid());
	data.setEncounterId(dto->encounterId.getValue(""));
	data.setRuleName(dto->ruleName.getValue(""));
	data.setTriggerSourceTable("wkf_activity_definition");
	data.setTriggerSourceId(dto->activityId.getValue(""));
	data.setChargeDefinitionId(dto->chargeDefinitionId.getValue(""));
	data.setQuantityValue(dto->quantityValue ? dto->quantityValue.getValue(1.0) : 1.0);
	data.setPerformingOrgId(dto->performingOrgId ? dto->performingOrgId.getValue("") : "");
	data.setRemark(dto->remark ? dto->remark.getValue("") : "");
	int priority = 0;
	if (dto->priority && !dto->priority->empty())
	{
		try { priority = std::stoi(dto->priority.getValue("0")); }
		catch (...) {}
	}
	data.setPriority(priority);
	data.setStatusEnum("ACTIVE");
	data.setIsDeleted(false);
	data.setVersion(0);
	data.setCreatedAt(SimpleDateTimeFormat::format());
	InpatientBillingDAO dao;
	dao.insert(data);
	response->ruleId = data.getId();
	return response;
}

UpdateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::updateAutoGenRule(const UpdateAutoGenRuleDTO::Wrapper& dto)
{
	auto response = UpdateAutoGenRuleResponseDTO::createShared();
	if (!dto || !dto->encounterId || dto->encounterId->empty()) return response;
	InpatientBillingDAO dao;
	auto rules = dao.selectRulesByEncounterId(dto->encounterId.getValue(""));
	for (auto& rule : rules)
	{
		if (rule.getId() != dto->ruleId.getValue("")) continue;
		if (dto->ruleName && !dto->ruleName->empty())
			rule.setRuleName(dto->ruleName.getValue(""));
		if (dto->activityId && !dto->activityId->empty())
		{
			rule.setTriggerSourceTable("wkf_activity_definition");
			rule.setTriggerSourceId(dto->activityId.getValue(""));
		}
		if (dto->chargeDefinitionId && !dto->chargeDefinitionId->empty())
			rule.setChargeDefinitionId(dto->chargeDefinitionId.getValue(""));
		if (dto->quantityValue)
			rule.setQuantityValue(dto->quantityValue.getValue(1.0));
		if (dto->performingOrgId)
			rule.setPerformingOrgId(dto->performingOrgId.getValue(""));
		if (dto->priority && !dto->priority->empty())
		{
			try { rule.setPriority(std::stoi(dto->priority.getValue("0"))); }
			catch (...) {}
		}
		if (dto->remark)
			rule.setRemark(dto->remark.getValue(""));
		rule.setUpdatedAt(SimpleDateTimeFormat::format());
		dao.update(rule);
		response->success = true;
		return response;
	}
	return response;
}

DeleteAutoGenRuleResponseDTO::Wrapper InpatientBillingService::deleteAutoGenRule(const DeleteAutoGenRuleDTO::Wrapper& dto)
{
	auto response = DeleteAutoGenRuleResponseDTO::createShared();
	if (!dto || !dto->encounterId || dto->encounterId->empty()) return response;
	InpatientBillingDAO dao;
	auto rules = dao.selectRulesByEncounterId(dto->encounterId.getValue(""));
	for (auto& rule : rules)
	{
		if (rule.getId() != dto->ruleId.getValue("")) continue;
		rule.setIsDeleted(true);
		rule.setDeletedAt(SimpleDateTimeFormat::format());
		dao.update(rule);
		response->success = true;
		return response;
	}
	return response;
}

ActivateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::activateAutoGenRule(const ActivateAutoGenRuleDTO::Wrapper& dto)
{
	auto response = ActivateAutoGenRuleResponseDTO::createShared();
	if (!dto || !dto->encounterId || dto->encounterId->empty()) return response;
	InpatientBillingDAO dao;
	auto rules = dao.selectRulesByEncounterId(dto->encounterId.getValue(""));
	for (auto& rule : rules)
	{
		if (rule.getId() != dto->ruleId.getValue("")) continue;
		rule.setStatusEnum("ACTIVE");
		rule.setUpdatedAt(SimpleDateTimeFormat::format());
		dao.update(rule);
		response->success = true;
		return response;
	}
	return response;
}

ActivateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::suspendAutoGenRule(const ActivateAutoGenRuleDTO::Wrapper& dto)
{
	auto response = ActivateAutoGenRuleResponseDTO::createShared();
	if (!dto || !dto->encounterId || dto->encounterId->empty()) return response;
	InpatientBillingDAO dao;
	auto rules = dao.selectRulesByEncounterId(dto->encounterId.getValue(""));
	for (auto& rule : rules)
	{
		if (rule.getId() != dto->ruleId.getValue("")) continue;
		rule.setStatusEnum("INACTIVE");
		rule.setUpdatedAt(SimpleDateTimeFormat::format());
		dao.update(rule);
		response->success = true;
		return response;
	}
	return response;
}
