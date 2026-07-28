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
#ifndef _CLN_DIAGNOSTIC_REPORT_INDEX_DO_H_
#define _CLN_DIAGNOSTIC_REPORT_INDEX_DO_H_

#include "../DoInclude.h"

/**
 * Database model for cln_diagnostic_report_index 诊断报告索引表
 */
class ClnDiagnosticIndexDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, documentId, DocumentId);
	MYSQL_SYNTHESIZE(string, moduleEnum, ModuleEnum);
	MYSQL_SYNTHESIZE(string, sourceReportId, SourceReportId);
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, publishedAt, PublishedAt);
	MYSQL_SYNTHESIZE(string, title, Title);
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	MYSQL_SYNTHESIZE(string, pdfStorageKey, PdfStorageKey);
	MYSQL_SYNTHESIZE(string, externalViewUrl, ExternalViewUrl);
	MYSQL_SYNTHESIZE(string, amendedFromId, AmendedFromId);
	MYSQL_SYNTHESIZE(string, uniqueKey, UniqueKey);

public:
	ClnDiagnosticIndexDO() : BaseDO("cln_diagnostic_report_index")
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
		MYSQL_ADD_FIELD_NULLABLE("document_id", "s", documentId, false);
		MYSQL_ADD_FIELD_NULLABLE("module_enum", "s", moduleEnum, false);
		MYSQL_ADD_FIELD("source_report_id", "s", sourceReportId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("published_at", "dt", publishedAt);
		MYSQL_ADD_FIELD("title", "s", title);
		MYSQL_ADD_FIELD("category_code", "s", categoryCode);
		MYSQL_ADD_FIELD("pdf_storage_key", "s", pdfStorageKey);
		MYSQL_ADD_FIELD("external_view_url", "s", externalViewUrl);
		MYSQL_ADD_FIELD("amended_from_id", "s", amendedFromId);
		MYSQL_ADD_FIELD_NULLABLE("unique_key", "s", uniqueKey, false);
	}
};

typedef std::shared_ptr<ClnDiagnosticIndexDO> PtrClnDiagnosticIndexDO;

#endif // !_CLN_DIAGNOSTIC_REPORT_INDEX_DO_H_#pragma once
