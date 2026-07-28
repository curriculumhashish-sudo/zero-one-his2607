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
#ifndef _EmrDocumentMapper_H_
#define _EmrDocumentMapper_H_

#include "Mapper.h"
#include "../../domain/do/outpatientrecord/EmrDocumentDO.h"

/**
 * EMR document result set mapper
 *
 * Converts a database ResultSet row into an EmrDocumentDO persistent object.
 * Maps all columns from the emr_document table to their corresponding DO fields.
 */
class EmrDocumentMapper : public Mapper<PtrEmrDocumentDO>
{
public:
	PtrEmrDocumentDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<EmrDocumentDO>();

		// Primary Key
		data->setId(resultSet->getString("id"));

		// Core Business Fields
		data->setPatientId(resultSet->getString("patient_id"));
		data->setEncounterId(resultSet->getString("encounter_id"));
		data->setDocumentTypeCode(resultSet->getString("document_type_code"));
		data->setTitle(resultSet->getString("title"));
		data->setContent(resultSet->getString("content"));
		data->setStatus(resultSet->getString("status"));
		data->setSourceTemplateId(resultSet->getString("source_template_id"));

		// Audit Fields
		data->setCreatedAt(resultSet->getString("created_at"));
		data->setUpdatedAt(resultSet->getString("updated_at"));
		data->setCreatedBy(resultSet->getString("created_by"));
		data->setUpdatedBy(resultSet->getString("updated_by"));
		data->setIsDeleted(resultSet->getInt("is_deleted"));
		data->setTenantId(resultSet->getString("tenant_id"));

		return data;
	}
};

#endif // !_EmrDocumentMapper_H_
