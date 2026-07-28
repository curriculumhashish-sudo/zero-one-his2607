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
#ifndef _EmrDocumentDAO_H_
#define _EmrDocumentDAO_H_

#include "BaseDAO.h"
#include "EmrDocumentMapper.h"
#include "../../domain/do/outpatientrecord/EmrDocumentDO.h"

/**
 * EMR document data access object
 *
 * Provides data access for the emr_document table.
 * Insert and update operations are inherited from BaseDAO.
 */
class EmrDocumentDAO : public BaseDAO
{
public:
	// Query a single document by its primary key, returns nullptr if not found
	PtrEmrDocumentDO selectById(const std::string& id);
};

#endif // !_EmrDocumentDAO_H_
