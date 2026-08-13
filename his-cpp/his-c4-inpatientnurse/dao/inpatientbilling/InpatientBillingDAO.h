#pragma once
#ifndef _INPATIENT_BILLING_DAO_H_
#define _INPATIENT_BILLING_DAO_H_

#include "BaseDAO.h"
#include "domain/do/inpatientbilling/AutoGenRuleDO.h"
#include "domain/do/chargedetails/ChargeItemDO.h"
#include "domain/dto/inpatientbilling/InpatientBillingDTO.h"

class InpatientBillingDAO : public BaseDAO
{
public:
	std::list<AutoGenRuleDO> selectRulesByEncounterId(const std::string& encounterId);
	uint64_t countRulesByEncounterId(const std::string& encounterId);
	bool isValidEncounterPatient(
		const std::string& encounterId, const std::string& patientId);
	bool isValidAutoRuleReferences(
		const std::string& activityId, const std::string& chargeDefinitionId);
	bool isActiveActivity(const std::string& activityId);
	bool isActiveChargeDefinition(const std::string& chargeDefinitionId);
	int updateAutoGenRule(const UpdateAutoGenRuleDTO::Wrapper& dto);
	int softDeleteAutoGenRule(const std::string& ruleId, const std::string& encounterId,
		int64_t expectedVersion);
	int setAutoGenRuleStatus(
		const std::string& ruleId, const std::string& encounterId, const std::string& status,
		int64_t expectedVersion);

	uint64_t countPricingCatalog(const PagePricingCatalogQuery::Wrapper& query);
	std::list<NurseStationPricingCatalogRowDTO::Wrapper> selectPricingCatalog(
		const PagePricingCatalogQuery::Wrapper& query);
	std::list<PricedOrderGroupDTO::Wrapper> selectPricedOrders(
		const ListPricedOrdersQuery::Wrapper& query);
	std::list<SupplementParentOrderGroupDTO::Wrapper> selectSupplementParentOrders(
		const ListSupplementParentOrdersQuery::Wrapper& query);
	bool prepareNursePricingItem(const ConfirmDirectPricingItemDTO::Wrapper& item);
	bool isAllowedSupplementParent(
		const std::string& requestId, const std::string& encounterId,
		const std::string& orderType);
	int insertNursePricingServiceRequest(
		const std::string& requestId,
		const ConfirmDirectPricingItemDTO::Wrapper& item,
		const std::string& parentRequestId,
		const std::string& operatorId,
		const std::string& now);
	int insertNursePricingDeviceRequest(
		const std::string& requestId,
		const ConfirmDirectPricingItemDTO::Wrapper& item,
		const std::string& parentRequestId,
		const std::string& operatorId,
		const std::string& now);
	int insertNursePricingDeviceDispense(
		const std::string& dispenseId,
		const std::string& requestId,
		const ConfirmDirectPricingItemDTO::Wrapper& item,
		const std::string& parentRequestId,
		const std::string& operatorId,
		const std::string& now);
	int insertNursePricingChargeItem(
		const std::string& chargeItemId,
		const std::string& requestId,
		const std::string& dispenseId,
		const ConfirmDirectPricingItemDTO::Wrapper& item,
		const std::string& operatorId,
		const std::string& now);
	int softDeleteNursePricingRequest(
		const std::string& orderType,
		const std::string& requestId,
		const std::string& encounterId,
		const std::string& operatorId,
		const std::string& now);
	int softDeletePendingChargeItems(
		const std::string& requestId,
		const std::string& encounterId,
		const std::string& operatorId,
		const std::string& now);
	std::list<std::string> selectIdempotencyResult(
		const std::string& operation, const std::string& idempotencyKey);
	int insertIdempotencyResult(
		const std::string& auditId, const std::string& operation,
		const std::string& idempotencyKey, const std::string& operatorId,
		const std::string& fingerprint, const std::string& resultData);

    std::unique_ptr<ChargeItemDO> selectChargeItemByCode(const std::string& itemCode);

    int insertChargeItem(const ChargeItemDO& chargeItem);

    std::list<ChargeItemDO> selectChargeItemsByPatientAndEncounter(
        const std::string& patientId,
        const std::string& encounterId
    );

    std::list<ChargeItemDO> selectChargeItemsByPatientId(
        const std::string& patientId,
        int offset,
        int limit
    );

    uint64_t countChargeItemsByPatientId(const std::string& patientId);

    std::unique_ptr<ChargeItemDO> selectChargeItemByServiceId(const std::string& serviceId);

    int updateChargeItemStatus(const std::string& id, const std::string& status);
};

#endif // !_INPATIENT_BILLING_DAO_H_
