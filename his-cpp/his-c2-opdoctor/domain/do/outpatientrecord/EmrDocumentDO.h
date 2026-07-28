#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2026/07/28

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
#ifndef _EmrDocumentDO_H_
#define _EmrDocumentDO_H_

#include "../DoInclude.h"

/**
 * EMR Document Data Object
 *
 * Database table: emr_document
 *
 * Maps the core electronic medical record document entity.
 * Used by the outpatient record save operation to insert or update
 * medical record documents with their Tiptap/ProseMirror JSON content.
 */
class EmrDocumentDO : public BaseDO
{
	// =============================
	// Primary Key
	// =============================
	// Document ID, SnowFlake-generated unique identifier
	MYSQL_SYNTHESIZE(string, id, Id);

	// =============================
	// Core Business Fields
	// =============================
	// Patient ID, references the patient this record belongs to
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// Encounter/Visit ID, references the outpatient visit
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// Document type code, e.g. OUTPATIENT_RECORD
	MYSQL_SYNTHESIZE(string, documentTypeCode, DocumentTypeCode);
	// Document title, auto-generated if not supplied
	MYSQL_SYNTHESIZE(string, title, Title);
	// Document body content in Tiptap/ProseMirror JSON format
	MYSQL_SYNTHESIZE(string, content, Content);
	// Document status: DRAFT / SUBMITTED / LOCKED / ARCHIVED
	MYSQL_SYNTHESIZE(string, status, Status);
	// Source template ID, references emr_template.id
	MYSQL_SYNTHESIZE(string, sourceTemplateId, SourceTemplateId);

	// =============================
	// Audit Fields
	// =============================
	// Created at timestamp
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// Updated at timestamp
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// Created by (operator username / ID)
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// Updated by (operator username / ID)
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// Soft delete flag: 0 = active, 1 = deleted
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// Tenant ID for multi-tenancy isolation
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);

public:
	EmrDocumentDO() : BaseDO("emr_document")
	{
		// Primary Key
		MYSQL_ADD_FIELD_PK("id", "s", id);

		// Core Business Fields
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("document_type_code", "s", documentTypeCode);
		MYSQL_ADD_FIELD("title", "s", title);
		MYSQL_ADD_FIELD("content", "s", content);
		MYSQL_ADD_FIELD("status", "s", status);
		MYSQL_ADD_FIELD("source_template_id", "s", sourceTemplateId);

		// Audit Fields
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
	}
};

// Shared pointer type alias
typedef std::shared_ptr<EmrDocumentDO> PtrEmrDocumentDO;

#endif // !_EmrDocumentDO_H_
