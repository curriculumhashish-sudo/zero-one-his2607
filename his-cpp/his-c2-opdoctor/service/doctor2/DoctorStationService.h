/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/08/09

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
#pragma once
#ifndef _DOCTOR_STATION_SERVICE_H_
#define _DOCTOR_STATION_SERVICE_H_

#include "domain/dto/doctor2/InpatientAdmissionFormOptionsDTO.h"
#include "domain/dto/PayloadDTO.h"

/**
 * Doctor station business logic service
 * Handles admission options, diagnosis dictionary, advice order catalog, etc.
 */
class DoctorStationService
{
public:
    /**
     * Load fixed options for the inpatient admission form
     * Returns department options, admission types, admission ways, and priority options
     * @param payload auth context with current user, org, and tenant info
     * @return aggregated options DTO with all four option lists
     */
    InpatientAdmissionFormOptionsDTO::Wrapper loadInpatientAdmissionOptions(const PayloadDTO& payload);
};

#endif // !_DOCTOR_STATION_SERVICE_H_
