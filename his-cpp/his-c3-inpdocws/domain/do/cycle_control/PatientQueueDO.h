#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:51:41

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
#ifndef _PATIENT_QUEUE_DO_H_
#define _PATIENT_QUEUE_DO_H_

#include "../DoInclude.h"

/**
 * 住院患者队列查询结果视图DO。
 * 映射多表JOIN查询的扁平化结果。
 */
class PatientQueueDO
{
	CC_SYNTHESIZE(string, encounterId, EncounterId);
	CC_SYNTHESIZE(string, patientId, PatientId);
	CC_SYNTHESIZE(string, patientName, PatientName);
	CC_SYNTHESIZE(string, gender, Gender);
	CC_SYNTHESIZE(int, age, Age);
	CC_SYNTHESIZE(string, diagnosisSummary, DiagnosisSummary);
	CC_SYNTHESIZE(string, bedName, BedName);
	CC_SYNTHESIZE(string, bedId, BedId);
	CC_SYNTHESIZE(string, wardId, WardId);
	CC_SYNTHESIZE(string, wardName, WardName);
	CC_SYNTHESIZE(string, houseName, HouseName);
	CC_SYNTHESIZE(double, accountBalance, AccountBalance);
	CC_SYNTHESIZE(string, statusEnum, StatusEnum);
	CC_SYNTHESIZE(string, encounterBusNo, EncounterBusNo);
	CC_SYNTHESIZE(string, admitTime, AdmitTime);
	CC_SYNTHESIZE(string, organizationId, OrganizationId);
	CC_SYNTHESIZE(string, organizationName, OrganizationName);
};

typedef std::shared_ptr<PatientQueueDO> PtrPatientQueueDO;

#endif // !_PATIENT_QUEUE_DO_H_
