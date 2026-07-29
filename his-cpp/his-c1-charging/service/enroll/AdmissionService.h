#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: yiguang
 @Date: 2026/07/28 00:00:00

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
#ifndef _ADMISSION_SERVICE_
#define _ADMISSION_SERVICE_

#include "domain/dto/enroll/admissionDTO.h"
#include "domain/dto/PayloadDTO.h"

class AdmissionService
{
public:
	// Inpatient admission registration
	bool insertAdmission(const AdmissionAddDTO::Wrapper& dto, const PayloadDTO& payload);

	// Cancel admission registration
	bool deleteAdmission(const AdmissionDeleteDTO::Wrapper& dto, const PayloadDTO& payload);
};

#endif // !_ADMISSION_SERVICE_
