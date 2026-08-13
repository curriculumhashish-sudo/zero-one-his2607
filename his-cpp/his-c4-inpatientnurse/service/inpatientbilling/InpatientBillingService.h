#pragma once
#ifndef _INPATIENT_BILLING_SERVICE_H_
#define _INPATIENT_BILLING_SERVICE_H_
#include "domain/vo/inpatientbilling/InpatientBillingVO.h"
#include "domain/dto/inpatientbilling/InpatientBillingDTO.h"

class InpatientBillingService
{
public:
	PagePricingCatalogDTO::Wrapper pagePricingCatalog(const PagePricingCatalogQuery::Wrapper& query);
	ConfirmDirectPricingResponseDTO::Wrapper confirmDirectPricing(
		const ConfirmDirectPricingDTO::Wrapper& dto, const std::string& operatorId);
	ListPricedOrdersDTO::Wrapper listPricedOrders(const ListPricedOrdersQuery::Wrapper& query);
	ListSupplementParentOrdersDTO::Wrapper listSupplementParentOrders(
		const ListSupplementParentOrdersQuery::Wrapper& query);
	ConfirmSupplementPricingResponseDTO::Wrapper confirmSupplementPricing(
		const ConfirmSupplementPricingDTO::Wrapper& dto, const std::string& operatorId);
	CancelPricingResponseDTO::Wrapper cancelPricing(
		const CancelPricingDTO::Wrapper& dto, const std::string& operatorId);

	// Query automatic billing configurations.
	ListAutoRollConfigsDTO::Wrapper listAutoRollConfigs(const ListAutoRollConfigsQuery::Wrapper& query);
	// Create an automatic billing rule.
	CreateAutoGenRuleResponseDTO::Wrapper createAutoGenRule(
		const CreateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId);
	// Update non-null automatic billing rule fields.
	UpdateAutoGenRuleResponseDTO::Wrapper updateAutoGenRule(
		const UpdateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId);
	// Soft-delete an automatic billing rule.
	DeleteAutoGenRuleResponseDTO::Wrapper deleteAutoGenRule(
		const DeleteAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId);
	// Activate an automatic billing rule.
	ActivateAutoGenRuleResponseDTO::Wrapper activateAutoGenRule(
		const ActivateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId);
	// Suspend an automatic billing rule.
	ActivateAutoGenRuleResponseDTO::Wrapper suspendAutoGenRule(
		const ActivateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId);
};

#endif // !_INPATIENT_BILLING_SERVICE_H_
