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
#ifndef _OutpatientrecordService_H_
#define _OutpatientrecordService_H_

#include "../../domain/dto/outpatientrecord/outpatientrecordSaveDTO.h"
#include "domain/vo/BaseJsonVO.h"
#include "../../dao/outpatientrecord/EmrDocumentDAO.h"

/**
 * Outpatient record business service layer
 *
 * Save medical record core flow:
 *   1. Validate required input parameters (patientId, visitId, templateId, content)
 *   2. If outpatientrecordId is empty: generate SnowFlake ID, build EmrDocumentDO, INSERT
 *   3. If outpatientrecordId is not empty: query existing record, UPDATE fields
 *   4. Auto-fill audit fields: timestamps, operator, tenant_id from JWT payload
 *   5. Return StringJsonVO with the saved record ID
 */
class OutpatientrecordService
{
	// EMR document DAO: provides insert/update/selectById for emr_document table
	EmrDocumentDAO emrDocumentDAO;

public:
	/**
	 * Save an outpatient medical record (insert or update)
	 *
	 * @param dto Save request DTO carrying outpatientrecordId, patientId, visitId, title, templateId, content
	 * @param payload JWT payload providing current operator username and org ID
	 * @return StringJsonVO with the saved record's ID as data
	 */
	StringJsonVO::Wrapper save(
		const outpatientrecordSaveDTO::Wrapper& dto,
		const PayloadDTO& payload);
};

#endif // !_OutpatientrecordService_H_
