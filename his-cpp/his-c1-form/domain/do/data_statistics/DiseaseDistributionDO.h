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
#ifndef _DISEASE_DISTRIBUTION_DO_
#define _DISEASE_DISTRIBUTION_DO_

#include "../DoInclude.h"

class DiseaseDistributionDO : public BaseDO {
	// Disease type name
	MYSQL_SYNTHESIZE(string, diseaseName, DiseaseName);
	// Patient count for this disease
	MYSQL_SYNTHESIZE(int, patientCount, PatientCount);
	// Percentage of total patients
	MYSQL_SYNTHESIZE(double, percentage, Percentage);

public:
	// Use virtual view name since data comes from multi-table JOIN aggregation query
	DiseaseDistributionDO() : BaseDO("v_disease_distribution") {
		MYSQL_ADD_FIELD_PK("name", "s", diseaseName);
		MYSQL_ADD_FIELD("patientCount", "i", patientCount);
		MYSQL_ADD_FIELD("percentage", "d", percentage);
	}
};

typedef std::shared_ptr<DiseaseDistributionDO> PtrDiseaseDistributionDO;

#endif // !_DISEASE_DISTRIBUTION_DO_
