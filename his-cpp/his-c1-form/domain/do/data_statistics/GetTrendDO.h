#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/20 00:00:00

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
#ifndef _GET_TREND_DO_
#define _GET_TREND_DO_
#include "../DoInclude.h"

class GetTrendDO : public BaseDO
{
	// Encounter record ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// Patient ID
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// Encounter start time
	MYSQL_SYNTHESIZE(string, startTime, StartTime);
	// Encounter end time
	MYSQL_SYNTHESIZE(string, endTime, EndTime);
	// Organization ID
	MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
	// Soft delete flag (0=valid, 1=deleted)
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// Status enum value
	MYSQL_SYNTHESIZE(string, statusNum, StatusNum);

public:
	GetTrendDO() : BaseDO("adm_encounter") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("organization_id", "s", organizationId);
		MYSQL_ADD_FIELD("start_time", "s", startTime);
		MYSQL_ADD_FIELD("end_time", "s", endTime);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("status_enum", "s", statusNum);
	}
};

typedef std::shared_ptr<GetTrendDO> PtrGetTrendDO;

#endif // !_GET_TREND_DO_
