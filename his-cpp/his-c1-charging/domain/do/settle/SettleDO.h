#pragma once

#ifndef _SETTLE_DO_H_
#define _SETTLE_DO_H_

#include "../DoInclude.h"

/**
 * 住院结算生成的支付流水。
 */
class SettlementPaymentDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(std::string, relationId, RelationId);
	MYSQL_SYNTHESIZE(std::string, paymentNo, PaymentNo);
	MYSQL_SYNTHESIZE(std::string, paymentEnum, PaymentEnum);
	MYSQL_SYNTHESIZE(std::string, kindEnum, KindEnum);
	MYSQL_SYNTHESIZE(std::string, entererId, EntererId);
	MYSQL_SYNTHESIZE(std::string, issuerEnum, IssuerEnum);
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	MYSQL_SYNTHESIZE(std::string, practitionerId, PractitionerId);
	MYSQL_SYNTHESIZE(std::string, outcomeEnum, OutcomeEnum);
	MYSQL_SYNTHESIZE(std::string, locationId, LocationId);
	MYSQL_SYNTHESIZE(std::string, expirationDate, ExpirationDate);
	MYSQL_SYNTHESIZE(std::string, tenderedAmount, TenderedAmount);
	MYSQL_SYNTHESIZE(std::string, returnedAmount, ReturnedAmount);
	MYSQL_SYNTHESIZE(std::string, displayAmount, DisplayAmount);
	MYSQL_SYNTHESIZE(int, printCount, PrintCount);
	MYSQL_SYNTHESIZE(std::string, contractNo, ContractNo);
	MYSQL_SYNTHESIZE(std::string, chargeItemIds, ChargeItemIds);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, billDate, BillDate);
	MYSQL_SYNTHESIZE(std::string, invoiceId, InvoiceId);
	MYSQL_SYNTHESIZE(std::string, accountId, AccountId);
	MYSQL_SYNTHESIZE(int, ybClearFlag, YbClearFlag);
	MYSQL_SYNTHESIZE(std::string, cashierSettlementId, CashierSettlementId);

public:
	SettlementPaymentDO() : BaseDO("fin_payment_reconciliation")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
		MYSQL_ADD_FIELD("relation_id", "s", relationId);
		MYSQL_ADD_FIELD_NULLABLE("payment_no", "s", paymentNo, false);
		MYSQL_ADD_FIELD_NULLABLE("payment_enum", "s", paymentEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("kind_enum", "s", kindEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("enterer_id", "s", entererId, false);
		MYSQL_ADD_FIELD("issuer_enum", "s", issuerEnum);
		MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);
		MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
		MYSQL_ADD_FIELD("outcome_enum", "s", outcomeEnum);
		MYSQL_ADD_FIELD("location_id", "s", locationId);
		MYSQL_ADD_FIELD("expiration_date", "s", expirationDate);
		MYSQL_ADD_FIELD_NULLABLE("tendered_amount", "s", tenderedAmount, false);
		MYSQL_ADD_FIELD("returned_amount", "s", returnedAmount);
		MYSQL_ADD_FIELD_NULLABLE("display_amount", "s", displayAmount, false);
		MYSQL_ADD_FIELD_NULLABLE("print_count", "i", printCount, false);
		MYSQL_ADD_FIELD("contract_no", "s", contractNo);
		MYSQL_ADD_FIELD_NULLABLE("charge_item_ids", "s", chargeItemIds, false);
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD("bill_date", "s", billDate);
		MYSQL_ADD_FIELD("invoice_id", "s", invoiceId);
		MYSQL_ADD_FIELD("account_id", "s", accountId);
		MYSQL_ADD_FIELD("yb_clear_flag", "i", ybClearFlag);
		MYSQL_ADD_FIELD("cashier_settlement_id", "s", cashierSettlementId);
	}
};

typedef std::shared_ptr<SettlementPaymentDO> PtrSettlementPaymentDO;

/**
 * 住院结算生成的支付渠道明细。
 */
class SettlementPaymentDetailDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, reconciliationId, ReconciliationId);
	MYSQL_SYNTHESIZE(std::string, predecessorId, PredecessorId);
	MYSQL_SYNTHESIZE(std::string, targetEnum, TargetEnum);
	MYSQL_SYNTHESIZE(std::string, accountId, AccountId);
	MYSQL_SYNTHESIZE(std::string, accountCode, AccountCode);
	MYSQL_SYNTHESIZE(std::string, payEnum, PayEnum);
	MYSQL_SYNTHESIZE(std::string, payLevelEnum, PayLevelEnum);
	MYSQL_SYNTHESIZE(std::string, amount, Amount);
	MYSQL_SYNTHESIZE(std::string, returnAmount, ReturnAmount);
	MYSQL_SYNTHESIZE(std::string, chargeAmount, ChargeAmount);
	MYSQL_SYNTHESIZE(std::string, payTransNo, PayTransNo);
	MYSQL_SYNTHESIZE(std::string, payTransText, PayTransText);
	MYSQL_SYNTHESIZE(std::string, payTransDate, PayTransDate);
	MYSQL_SYNTHESIZE(std::string, beforeBalance, BeforeBalance);
	MYSQL_SYNTHESIZE(std::string, afterBalance, AfterBalance);
	MYSQL_SYNTHESIZE(std::string, resultEnum, ResultEnum);

public:
	SettlementPaymentDetailDO() : BaseDO("fin_payment_rec_detail")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("reconciliation_id", "s", reconciliationId, false);
		MYSQL_ADD_FIELD("predecessor_id", "s", predecessorId);
		MYSQL_ADD_FIELD("target_enum", "s", targetEnum);
		MYSQL_ADD_FIELD_NULLABLE("account_id", "s", accountId, false);
		MYSQL_ADD_FIELD("account_code", "s", accountCode);
		MYSQL_ADD_FIELD_NULLABLE("pay_enum", "s", payEnum, false);
		MYSQL_ADD_FIELD("pay_level_enum", "s", payLevelEnum);
		MYSQL_ADD_FIELD_NULLABLE("amount", "s", amount, false);
		MYSQL_ADD_FIELD("return_amount", "s", returnAmount);
		MYSQL_ADD_FIELD("charge_amount", "s", chargeAmount);
		MYSQL_ADD_FIELD("pay_trans_no", "s", payTransNo);
		MYSQL_ADD_FIELD("pay_trans_text", "s", payTransText);
		MYSQL_ADD_FIELD("pay_trans_date", "s", payTransDate);
		MYSQL_ADD_FIELD("before_balance", "s", beforeBalance);
		MYSQL_ADD_FIELD("after_balance", "s", afterBalance);
		MYSQL_ADD_FIELD_NULLABLE("result_enum", "s", resultEnum, false);
	}
};

typedef std::shared_ptr<SettlementPaymentDetailDO> PtrSettlementPaymentDetailDO;

/**
 * 住院结算生成的发票。
 */
class SettlementInvoiceDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	MYSQL_SYNTHESIZE(std::string, reconciliationId, ReconciliationId);
	MYSQL_SYNTHESIZE(std::string, paymentId, PaymentId);
	MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
	MYSQL_SYNTHESIZE(std::string, billNo, BillNo);
	MYSQL_SYNTHESIZE(std::string, typeCode, TypeCode);
	MYSQL_SYNTHESIZE(std::string, cancelledReason, CancelledReason);
	MYSQL_SYNTHESIZE(std::string, chargeItemIds, ChargeItemIds);
	MYSQL_SYNTHESIZE(std::string, paymentTerms, PaymentTerms);
	MYSQL_SYNTHESIZE(std::string, amount, Amount);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(std::string, issuedAt, IssuedAt);

public:
	SettlementInvoiceDO() : BaseDO("adm_invoice")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("reconciliation_id", "s", reconciliationId);
		MYSQL_ADD_FIELD("payment_id", "s", paymentId);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("bill_no", "s", billNo);
		MYSQL_ADD_FIELD("type_code", "s", typeCode);
		MYSQL_ADD_FIELD("cancelled_reason", "s", cancelledReason);
		MYSQL_ADD_FIELD("charge_item_ids", "s", chargeItemIds);
		MYSQL_ADD_FIELD("payment_terms", "s", paymentTerms);
		MYSQL_ADD_FIELD("amount", "s", amount);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("issued_at", "s", issuedAt);
	}
};

typedef std::shared_ptr<SettlementInvoiceDO> PtrSettlementInvoiceDO;

/**
 * 用于结算校验及状态更新的收费项目实体。
 */
class SettlementChargeItemDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, accountId, AccountId);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(std::string, totalPrice, TotalPrice);

public:
	SettlementChargeItemDO() : BaseDO("adm_charge_item")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("account_id", "s", accountId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("total_price", "s", totalPrice);
	}
};

typedef std::shared_ptr<SettlementChargeItemDO> PtrSettlementChargeItemDO;

/**
 * 用于校验结算归属关系及结账状态的就诊账户实体。
 */
class SettlementAccountDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, patientId, PatientId);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, busNo, BusNo);
	MYSQL_SYNTHESIZE(std::string, balance, Balance);
	MYSQL_SYNTHESIZE(std::string, billingStatusEnum, BillingStatusEnum);
	MYSQL_SYNTHESIZE(std::string, contractNo, ContractNo);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);

public:
	SettlementAccountDO() : BaseDO("adm_account")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("balance", "s", balance);
		MYSQL_ADD_FIELD("billing_status_enum", "s", billingStatusEnum);
		MYSQL_ADD_FIELD("contract_no", "s", contractNo);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
	}
};

typedef std::shared_ptr<SettlementAccountDO> PtrSettlementAccountDO;

/**
 * 支付流水与医保正式结算记录的关联实体。
 */
class SettlementYbLinkDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, reconciliationId, ReconciliationId);
	MYSQL_SYNTHESIZE(std::string, setlId, SetlId);
	MYSQL_SYNTHESIZE(std::string, chrgBchno, ChrgBchno);
	MYSQL_SYNTHESIZE(std::string, mdtrtId, MdtrtId);
	MYSQL_SYNTHESIZE(std::string, settlementRecordId, SettlementRecordId);
	MYSQL_SYNTHESIZE(std::string, accountId, AccountId);
	MYSQL_SYNTHESIZE(std::string, status, Status);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);

public:
	SettlementYbLinkDO() : BaseDO("fin_payment_yb_settlement")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("reconciliation_id", "s", reconciliationId, false);
		MYSQL_ADD_FIELD_NULLABLE("setl_id", "s", setlId, false);
		MYSQL_ADD_FIELD("chrg_bchno", "s", chrgBchno);
		MYSQL_ADD_FIELD("mdtrt_id", "s", mdtrtId);
		MYSQL_ADD_FIELD("settlement_record_id", "s", settlementRecordId);
		MYSQL_ADD_FIELD("account_id", "s", accountId);
		MYSQL_ADD_FIELD_NULLABLE("status", "s", status, false);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
	}
};

typedef std::shared_ptr<SettlementYbLinkDO> PtrSettlementYbLinkDO;

/**
 * 正式结算使用的住院医保预结算记录。
 */
class InpatientPreSettleDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, paymentDraftId, PaymentDraftId);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, contractId, ContractId);
	MYSQL_SYNTHESIZE(std::string, contractBusNo, ContractBusNo);
	MYSQL_SYNTHESIZE(std::string, mdtrtId, MdtrtId);
	MYSQL_SYNTHESIZE(std::string, psnNo, PsnNo);
	MYSQL_SYNTHESIZE(std::string, medType, MedType);
	MYSQL_SYNTHESIZE(std::string, midSetlFlag, MidSetlFlag);
	MYSQL_SYNTHESIZE(std::string, settleMode, SettleMode);
	MYSQL_SYNTHESIZE(std::string, medfeeSumamt, MedfeeSumamt);
	MYSQL_SYNTHESIZE(std::string, fundPaySumamt, FundPaySumamt);
	MYSQL_SYNTHESIZE(std::string, acctPay, AcctPay);
	MYSQL_SYNTHESIZE(std::string, psnCashPay, PsnCashPay);
	MYSQL_SYNTHESIZE(std::string, chargeItemIds, ChargeItemIds);
	MYSQL_SYNTHESIZE(std::string, setlId, SetlId);
	MYSQL_SYNTHESIZE(std::string, status, Status);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, accountId, AccountId);
	MYSQL_SYNTHESIZE(std::string, insuplcAdmdvs, InsuplcAdmdvs);
	MYSQL_SYNTHESIZE(std::string, mdtrtCertType, MdtrtCertType);
	MYSQL_SYNTHESIZE(std::string, mdtrtCertNo, MdtrtCertNo);
	MYSQL_SYNTHESIZE(std::string, insutype, Insutype);
	MYSQL_SYNTHESIZE(std::string, acctUsedFlag, AcctUsedFlag);
	MYSQL_SYNTHESIZE(std::string, psnSetlway, PsnSetlway);
	MYSQL_SYNTHESIZE(std::string, fulamtOwnpayAmt, FulamtOwnpayAmt);
	MYSQL_SYNTHESIZE(std::string, overlmtSelfpay, OverlmtSelfpay);
	MYSQL_SYNTHESIZE(std::string, preselfpayAmt, PreselfpayAmt);
	MYSQL_SYNTHESIZE(std::string, inscpScpAmt, InscpScpAmt);

public:
	InpatientPreSettleDO() : BaseDO("yb_inpatient_pre_settle")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("payment_draft_id", "s", paymentDraftId, false);
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD_NULLABLE("contract_id", "s", contractId, false);
		MYSQL_ADD_FIELD_NULLABLE("contract_bus_no", "s", contractBusNo, false);
		MYSQL_ADD_FIELD_NULLABLE("mdtrt_id", "s", mdtrtId, false);
		MYSQL_ADD_FIELD_NULLABLE("psn_no", "s", psnNo, false);
		MYSQL_ADD_FIELD("med_type", "s", medType);
		MYSQL_ADD_FIELD("mid_setl_flag", "s", midSetlFlag);
		MYSQL_ADD_FIELD("settle_mode", "s", settleMode);
		MYSQL_ADD_FIELD("medfee_sumamt", "s", medfeeSumamt);
		MYSQL_ADD_FIELD("fund_pay_sumamt", "s", fundPaySumamt);
		MYSQL_ADD_FIELD("acct_pay", "s", acctPay);
		MYSQL_ADD_FIELD("psn_cash_pay", "s", psnCashPay);
		MYSQL_ADD_FIELD("charge_item_ids", "s", chargeItemIds);
		MYSQL_ADD_FIELD("setl_id", "s", setlId);
		MYSQL_ADD_FIELD_NULLABLE("status", "s", status, false);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, false);
		MYSQL_ADD_FIELD("account_id", "s", accountId);
		MYSQL_ADD_FIELD("insuplc_admdvs", "s", insuplcAdmdvs);
		MYSQL_ADD_FIELD("mdtrt_cert_type", "s", mdtrtCertType);
		MYSQL_ADD_FIELD("mdtrt_cert_no", "s", mdtrtCertNo);
		MYSQL_ADD_FIELD("insutype", "s", insutype);
		MYSQL_ADD_FIELD("acct_used_flag", "s", acctUsedFlag);
		MYSQL_ADD_FIELD("psn_setlway", "s", psnSetlway);
		MYSQL_ADD_FIELD("fulamt_ownpay_amt", "s", fulamtOwnpayAmt);
		MYSQL_ADD_FIELD("overlmt_selfpay", "s", overlmtSelfpay);
		MYSQL_ADD_FIELD("preselfpay_amt", "s", preselfpayAmt);
		MYSQL_ADD_FIELD("inscp_scp_amt", "s", inscpScpAmt);
	}
};

typedef std::shared_ptr<InpatientPreSettleDO> PtrInpatientPreSettleDO;

/**
 * 住院医保正式结算记录。
 */
class InpatientYbSettlementRecordDO : public BaseDO
{
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, contractId, ContractId);
	MYSQL_SYNTHESIZE(std::string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(std::string, paymentId, PaymentId);
	MYSQL_SYNTHESIZE(std::string, preSettleId, PreSettleId);
	MYSQL_SYNTHESIZE(std::string, chrgBchno, ChrgBchno);
	MYSQL_SYNTHESIZE(std::string, setlId, SetlId);
	MYSQL_SYNTHESIZE(std::string, mdtrtId, MdtrtId);
	MYSQL_SYNTHESIZE(std::string, psnNo, PsnNo);
	MYSQL_SYNTHESIZE(std::string, medfeeSumamt, MedfeeSumamt);
	MYSQL_SYNTHESIZE(std::string, fundPaySumamt, FundPaySumamt);
	MYSQL_SYNTHESIZE(std::string, acctPay, AcctPay);
	MYSQL_SYNTHESIZE(std::string, psnCashPay, PsnCashPay);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, accountId, AccountId);
	MYSQL_SYNTHESIZE(std::string, encounterClass, EncounterClass);
	MYSQL_SYNTHESIZE(std::string, status, Status);
	MYSQL_SYNTHESIZE(std::string, settleMode, SettleMode);
	MYSQL_SYNTHESIZE(std::string, psnName, PsnName);
	MYSQL_SYNTHESIZE(std::string, insutype, Insutype);
	MYSQL_SYNTHESIZE(std::string, medType, MedType);
	MYSQL_SYNTHESIZE(std::string, setlTime, SetlTime);
	MYSQL_SYNTHESIZE(std::string, fulamtOwnpayAmt, FulamtOwnpayAmt);
	MYSQL_SYNTHESIZE(std::string, overlmtSelfpay, OverlmtSelfpay);
	MYSQL_SYNTHESIZE(std::string, preselfpayAmt, PreselfpayAmt);
	MYSQL_SYNTHESIZE(std::string, inscpScpAmt, InscpScpAmt);
	MYSQL_SYNTHESIZE(std::string, hifpPay, HifpPay);
	MYSQL_SYNTHESIZE(std::string, cvlservPay, CvlservPay);
	MYSQL_SYNTHESIZE(std::string, hifmiPay, HifmiPay);
	MYSQL_SYNTHESIZE(std::string, hifobPay, HifobPay);
	MYSQL_SYNTHESIZE(std::string, hifesPay, HifesPay);
	MYSQL_SYNTHESIZE(std::string, cvlservFlag, CvlservFlag);
	MYSQL_SYNTHESIZE(std::string, poolPropSelfpay, PoolPropSelfpay);
	MYSQL_SYNTHESIZE(std::string, mafPay, MafPay);
	MYSQL_SYNTHESIZE(std::string, othPay, OthPay);
	MYSQL_SYNTHESIZE(std::string, psnPartAmt, PsnPartAmt);
	MYSQL_SYNTHESIZE(std::string, hospPartAmt, HospPartAmt);
	MYSQL_SYNTHESIZE(std::string, balc, Balc);
	MYSQL_SYNTHESIZE(std::string, acctMulaidPay, AcctMulaidPay);
	MYSQL_SYNTHESIZE(std::string, psnType, PsnType);
	MYSQL_SYNTHESIZE(std::string, actPayDedc, ActPayDedc);
	MYSQL_SYNTHESIZE(std::string, hifdmPay, HifdmPay);
	MYSQL_SYNTHESIZE(std::string, expContent, ExpContent);
	MYSQL_SYNTHESIZE(std::string, medinsSetlId, MedinsSetlId);
	MYSQL_SYNTHESIZE(std::string, msgid2207, Msgid2207);
	MYSQL_SYNTHESIZE(std::string, clrOptins, ClrOptins);
	MYSQL_SYNTHESIZE(std::string, clrWay, ClrWay);
	MYSQL_SYNTHESIZE(std::string, clrType, ClrType);
	MYSQL_SYNTHESIZE(std::string, fundBreakdownJson, FundBreakdownJson);

public:
	InpatientYbSettlementRecordDO() : BaseDO("yb_settlement_record")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("contract_id", "s", contractId, false);
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD("payment_id", "s", paymentId);
		MYSQL_ADD_FIELD("pre_settle_id", "s", preSettleId);
		MYSQL_ADD_FIELD_NULLABLE("chrg_bchno", "s", chrgBchno, false);
		MYSQL_ADD_FIELD_NULLABLE("setl_id", "s", setlId, false);
		MYSQL_ADD_FIELD_NULLABLE("mdtrt_id", "s", mdtrtId, false);
		MYSQL_ADD_FIELD_NULLABLE("psn_no", "s", psnNo, false);
		MYSQL_ADD_FIELD("medfee_sumamt", "s", medfeeSumamt);
		MYSQL_ADD_FIELD("fund_pay_sumamt", "s", fundPaySumamt);
		MYSQL_ADD_FIELD("acct_pay", "s", acctPay);
		MYSQL_ADD_FIELD("psn_cash_pay", "s", psnCashPay);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, false);
		MYSQL_ADD_FIELD("account_id", "s", accountId);
		MYSQL_ADD_FIELD_NULLABLE("encounter_class", "s", encounterClass, false);
		MYSQL_ADD_FIELD_NULLABLE("status", "s", status, false);
		MYSQL_ADD_FIELD("settle_mode", "s", settleMode);
		MYSQL_ADD_FIELD("psn_name", "s", psnName);
		MYSQL_ADD_FIELD("insutype", "s", insutype);
		MYSQL_ADD_FIELD("med_type", "s", medType);
		MYSQL_ADD_FIELD("setl_time", "s", setlTime);
		MYSQL_ADD_FIELD("fulamt_ownpay_amt", "s", fulamtOwnpayAmt);
		MYSQL_ADD_FIELD("overlmt_selfpay", "s", overlmtSelfpay);
		MYSQL_ADD_FIELD("preselfpay_amt", "s", preselfpayAmt);
		MYSQL_ADD_FIELD("inscp_scp_amt", "s", inscpScpAmt);
		MYSQL_ADD_FIELD("hifp_pay", "s", hifpPay);
		MYSQL_ADD_FIELD("cvlserv_pay", "s", cvlservPay);
		MYSQL_ADD_FIELD("hifmi_pay", "s", hifmiPay);
		MYSQL_ADD_FIELD("hifob_pay", "s", hifobPay);
		MYSQL_ADD_FIELD("hifes_pay", "s", hifesPay);
		MYSQL_ADD_FIELD("cvlserv_flag", "s", cvlservFlag);
		MYSQL_ADD_FIELD("pool_prop_selfpay", "s", poolPropSelfpay);
		MYSQL_ADD_FIELD("maf_pay", "s", mafPay);
		MYSQL_ADD_FIELD("oth_pay", "s", othPay);
		MYSQL_ADD_FIELD("psn_part_amt", "s", psnPartAmt);
		MYSQL_ADD_FIELD("hosp_part_amt", "s", hospPartAmt);
		MYSQL_ADD_FIELD("balc", "s", balc);
		MYSQL_ADD_FIELD("acct_mulaid_pay", "s", acctMulaidPay);
		MYSQL_ADD_FIELD("psn_type", "s", psnType);
		MYSQL_ADD_FIELD("act_pay_dedc", "s", actPayDedc);
		MYSQL_ADD_FIELD("hifdm_pay", "s", hifdmPay);
		MYSQL_ADD_FIELD("exp_content", "s", expContent);
		MYSQL_ADD_FIELD("medins_setl_id", "s", medinsSetlId);
		MYSQL_ADD_FIELD("msgid_2207", "s", msgid2207);
		MYSQL_ADD_FIELD("clr_optins", "s", clrOptins);
		MYSQL_ADD_FIELD("clr_way", "s", clrWay);
		MYSQL_ADD_FIELD("clr_type", "s", clrType);
		MYSQL_ADD_FIELD("fund_breakdown_json", "s", fundBreakdownJson);
	}
};

typedef std::shared_ptr<InpatientYbSettlementRecordDO> PtrInpatientYbSettlementRecordDO;

#endif // !_SETTLE_DO_H_
