#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei

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
#ifndef _DIAGNOSIS_DAO_H_
#define _DIAGNOSIS_DAO_H_

#include "BaseDAO.h"
#include "../../domain/do/Diagnosis/DiagnosisDO.h"
#include "../../domain/do/Diagnosis/ClnConditionDefinitionDO.h"
#include "../../domain/query/Diagnosis/DiagnosisSelectQuery.h"

/**
 * Data access object for diagnosis records.
 */
class DiagnosisDAO : public BaseDAO
{
private:
	std::string queryConditionBuilder(const DiagnosisSelectQuery::Wrapper& query, SqlParams& params);

public:
	uint64_t count(const DiagnosisSelectQuery::Wrapper& query);
	std::list<ClnConditionDefinitionDO> selectWithPage(const DiagnosisSelectQuery::Wrapper& query);
	std::list<DiagnosisDO> selectByName(const std::string& name);
};

#endif // !_DIAGNOSIS_DAO_H_
