/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/24

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
#ifndef _CLN_DIAGNOSTIC_REPORT_DO_H_
#define _CLN_DIAGNOSTIC_REPORT_DO_H_

#include "../DoInclude.h"

/**
 * Database model for cln_diagnostic_report ¡Ÿ¥≤◊‹ºÏ’Ô∂œ±®∏Ê
 */
class ClnDiagnosticDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, status, Status);
	MYSQL_SYNTHESIZE(string, summaryText, SummaryText);
	MYSQL_SYNTHESIZE(string, conclusionText, ConclusionText);
	MYSQL_SYNTHESIZE(string, resultRefsJson, ResultRefsJson);
	MYSQL_SYNTHESIZE(string, pdfStorageKey, PdfStorageKey);
	MYSQL_SYNTHESIZE(string, finalizedAt, FinalizedAt);
	MYSQL_SYNTHESIZE(string, finalizedBy, FinalizedBy);

public:
	ClnDiagnosticDO() : BaseDO("cln_diagnostic_report")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("org_id", "s", orgId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "b", isDeleted, false);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);
		MYSQL_ADD_FIELD_NULLABLE("encounter_id", "s", encounterId, false);
		MYSQL_ADD_FIELD_NULLABLE("patient_id", "s", patientId, false);
		MYSQL_ADD_FIELD("status", "s", status);
		MYSQL_ADD_FIELD("summary_text", "s", summaryText);
		MYSQL_ADD_FIELD("conclusion_text", "s", conclusionText);
		MYSQL_ADD_FIELD("result_refs_json", "s", resultRefsJson);
		MYSQL_ADD_FIELD("pdf_storage_key", "s", pdfStorageKey);
		MYSQL_ADD_FIELD("finalized_at", "dt", finalizedAt);
		MYSQL_ADD_FIELD("finalized_by", "s", finalizedBy);
	}
};

typedef std::shared_ptr<ClnDiagnosticDO> PtrClnDiagnosticDO;

#endif // !_CLN_DIAGNOSTIC_REPORT_DO_H_#pragma once
