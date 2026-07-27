#pragma once
#ifndef _CHARGEITEMDO_
#define _CHARGEITEMDO_

#include "../DoInclude.h"

class ChargeItemDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, generateSourceEnum, GenerateSourceEnum);
	MYSQL_SYNTHESIZE(std::string, contextEnum, ContextEnum);
	MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
	MYSQL_SYNTHESIZE(std::string, parentId, ParentId);
	MYSQL_SYNTHESIZE(std::string, prescriptionNo, PrescriptionNo);
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, serviceTable, ServiceTable);
	MYSQL_SYNTHESIZE(std::string, serviceId, ServiceId);
	MYSQL_SYNTHESIZE(std::string, accountId, AccountId);
	MYSQL_SYNTHESIZE(std::string, definitionId, DefinitionId);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(double, totalPrice, TotalPrice);
	MYSQL_SYNTHESIZE(std::string, occurrenceTime, OccurrenceTime);
	MYSQL_SYNTHESIZE(std::string, performerId, PerformerId);
	MYSQL_SYNTHESIZE(std::string, performingOrgId, PerformingOrgId);
	MYSQL_SYNTHESIZE(std::string, requestingOrgId, RequestingOrgId);
	MYSQL_SYNTHESIZE(std::string, costOrgId, CostOrgId);
	MYSQL_SYNTHESIZE(double, quantityValue, QuantityValue);
	MYSQL_SYNTHESIZE(std::string, quantityUnit, QuantityUnit);
	MYSQL_SYNTHESIZE(double, unitPrice, UnitPrice);
	MYSQL_SYNTHESIZE(std::string, defDetailId, DefDetailId);
	MYSQL_SYNTHESIZE(double, baseAmount, BaseAmount);
	MYSQL_SYNTHESIZE(double, discountAmount, DiscountAmount);
	MYSQL_SYNTHESIZE(double, surchargeAmount, SurchargeAmount);
	MYSQL_SYNTHESIZE(double, systemDiscountPrice, SystemDiscountPrice);
	MYSQL_SYNTHESIZE(double, manualAdjustedPrice, ManualAdjustedPrice);
	MYSQL_SYNTHESIZE(std::string, overrideReasonCode, OverrideReasonCode);
	MYSQL_SYNTHESIZE(std::string, overrideReasonText, OverrideReasonText);
	MYSQL_SYNTHESIZE(std::string, entererId, EntererId);
	MYSQL_SYNTHESIZE(std::string, enteredDate, EnteredDate);
	MYSQL_SYNTHESIZE(std::string, productTable, ProductTable);
	MYSQL_SYNTHESIZE(std::string, productId, ProductId);
	MYSQL_SYNTHESIZE(std::string, procedureId, ProcedureId);
	MYSQL_SYNTHESIZE(std::string, dispenseTable, DispenseTable);
	MYSQL_SYNTHESIZE(std::string, dispenseId, DispenseId);
	MYSQL_SYNTHESIZE(int, claimStateEnum, ClaimStateEnum);
	MYSQL_SYNTHESIZE(int, printCount, PrintCount);
	MYSQL_SYNTHESIZE(std::string, orgId, OrgId);
	MYSQL_SYNTHESIZE(std::string, refundId, RefundId);
	MYSQL_SYNTHESIZE(std::string, conditionId, ConditionId);
	MYSQL_SYNTHESIZE(std::string, encounterDiagnosisId, EncounterDiagnosisId);
	MYSQL_SYNTHESIZE(bool, tcmFlag, TcmFlag);

public:
	ChargeItemDO() : BaseDO("adm_charge_item")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD_NULLABLE("generate_source_enum", "s", generateSourceEnum);
		MYSQL_ADD_FIELD_NULLABLE("context_enum", "s", contextEnum);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo);
		MYSQL_ADD_FIELD_NULLABLE("parent_id", "s", parentId);
		MYSQL_ADD_FIELD_NULLABLE("prescription_no", "s", prescriptionNo);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD_NULLABLE("service_table", "s", serviceTable);
		MYSQL_ADD_FIELD_NULLABLE("service_id", "s", serviceId);
		MYSQL_ADD_FIELD_NULLABLE("account_id", "s", accountId);
		MYSQL_ADD_FIELD_NULLABLE("definition_id", "s", definitionId);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD_NULLABLE("total_price", "d", totalPrice);
		MYSQL_ADD_FIELD_NULLABLE("occurrence_time", "s", occurrenceTime);
		MYSQL_ADD_FIELD_NULLABLE("performer_id", "s", performerId);
		MYSQL_ADD_FIELD_NULLABLE("performing_org_id", "s", performingOrgId);
		MYSQL_ADD_FIELD_NULLABLE("requesting_org_id", "s", requestingOrgId);
		MYSQL_ADD_FIELD_NULLABLE("cost_org_id", "s", costOrgId);
		MYSQL_ADD_FIELD_NULLABLE("quantity_value", "d", quantityValue);
		MYSQL_ADD_FIELD_NULLABLE("quantity_unit", "s", quantityUnit);
		MYSQL_ADD_FIELD_NULLABLE("unit_price", "d", unitPrice);
		MYSQL_ADD_FIELD_NULLABLE("def_detail_id", "s", defDetailId);
		MYSQL_ADD_FIELD_NULLABLE("base_amount", "d", baseAmount);
		MYSQL_ADD_FIELD_NULLABLE("discount_amount", "d", discountAmount);
		MYSQL_ADD_FIELD_NULLABLE("surcharge_amount", "d", surchargeAmount);
		MYSQL_ADD_FIELD_NULLABLE("system_discount_price", "d", systemDiscountPrice);
		MYSQL_ADD_FIELD_NULLABLE("manual_adjusted_price", "d", manualAdjustedPrice);
		MYSQL_ADD_FIELD_NULLABLE("override_reason_code", "s", overrideReasonCode);
		MYSQL_ADD_FIELD_NULLABLE("override_reason_text", "s", overrideReasonText);
		MYSQL_ADD_FIELD_NULLABLE("enterer_id", "s", entererId);
		MYSQL_ADD_FIELD_NULLABLE("entered_date", "s", enteredDate);
		MYSQL_ADD_FIELD_NULLABLE("product_table", "s", productTable);
		MYSQL_ADD_FIELD_NULLABLE("product_id", "s", productId);
		MYSQL_ADD_FIELD_NULLABLE("procedure_id", "s", procedureId);
		MYSQL_ADD_FIELD_NULLABLE("dispense_table", "s", dispenseTable);
		MYSQL_ADD_FIELD_NULLABLE("dispense_id", "s", dispenseId);
		MYSQL_ADD_FIELD_NULLABLE("claim_state_enum", "i", claimStateEnum);
		MYSQL_ADD_FIELD_NULLABLE("print_count", "i", printCount);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId);
		MYSQL_ADD_FIELD_NULLABLE("refund_id", "s", refundId);
		MYSQL_ADD_FIELD_NULLABLE("condition_id", "s", conditionId);
		MYSQL_ADD_FIELD_NULLABLE("encounter_diagnosis_id", "s", encounterDiagnosisId);
		MYSQL_ADD_FIELD_NULLABLE("tcm_flag", "b", tcmFlag);
	}
};

typedef std::shared_ptr<ChargeItemDO> PtrChargeItemDO;

#endif
