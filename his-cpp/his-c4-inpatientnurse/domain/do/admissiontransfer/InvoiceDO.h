#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/27 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _INVOICEDO_H_
#define _INVOICEDO_H_

#include "../DoInclude.h"

// Complete mapping for database table adm_invoice.
class InvoiceDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, reconciliationId, ReconciliationId);
	MYSQL_SYNTHESIZE(string, paymentId, PaymentId);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, billNo, BillNo);
	MYSQL_SYNTHESIZE(string, typeCode, TypeCode);
	MYSQL_SYNTHESIZE(string, cancelledReason, CancelledReason);
	MYSQL_SYNTHESIZE(string, chargeItemIds, ChargeItemIds);
	MYSQL_SYNTHESIZE(string, paymentTerms, PaymentTerms);
	MYSQL_SYNTHESIZE(double, amount, Amount);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, issuedAt, IssuedAt);

public:
	InvoiceDO() : BaseDO("adm_invoice")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, true);
		MYSQL_ADD_FIELD_NULLABLE("reconciliation_id", "s", reconciliationId, true);
		MYSQL_ADD_FIELD_NULLABLE("payment_id", "s", paymentId, true);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
		MYSQL_ADD_FIELD_NULLABLE("bill_no", "s", billNo, true);
		MYSQL_ADD_FIELD_NULLABLE("type_code", "s", typeCode, true);
		MYSQL_ADD_FIELD_NULLABLE("cancelled_reason", "s", cancelledReason, true);
		MYSQL_ADD_FIELD_NULLABLE("charge_item_ids", "s", chargeItemIds, true);
		MYSQL_ADD_FIELD_NULLABLE("payment_terms", "s", paymentTerms, true);
		MYSQL_ADD_FIELD_NULLABLE("amount", "d", amount, true);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("issued_at", "dt", issuedAt, true);
	}
};

typedef std::shared_ptr<InvoiceDO> PtrInvoiceDO;

#endif // _INVOICEDO_H_
