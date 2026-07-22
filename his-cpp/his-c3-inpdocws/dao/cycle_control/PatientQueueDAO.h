#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:23:49

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
#ifndef _PATIENT_QUEUE_DAO_H_
#define _PATIENT_QUEUE_DAO_H_

#include "BaseDAO.h"
#include "../../domain/do/cycle_control/PatientQueueDO.h"
#include "../../domain/query/cycle_control/PatientQueueQuery.h"

/**
 * 住院患者队列查询DAO，基于多表JOIN实现。
 */
class PatientQueueDAO : public BaseDAO
{
public:
	uint64_t count(const PatientQueueQuery::Wrapper& query);
	list<PatientQueueDO> selectWithPage(const PatientQueueQuery::Wrapper& query);
private:
	string queryConditionBuilder(const PatientQueueQuery::Wrapper& query, SqlParams& params);
};

#endif // !_PATIENT_QUEUE_DAO_H_
