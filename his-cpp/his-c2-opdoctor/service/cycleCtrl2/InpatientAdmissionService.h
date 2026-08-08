#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/08/08 10:00:00

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
#ifndef _INPATIENTADMISSIONSERVICE_H_
#define _INPATIENTADMISSIONSERVICE_H_

#include "domain/vo/cycleCtrl2-admission/InpatientAdmissionWardVO.h"
#include <list>

/**
 * Business service for loading the ward options of a target inpatient
 * department when the doctor station handles an admission application.
 */
class InpatientAdmissionService
{
public:
	/**
	 * Load the enabled wards owned by the target department.
	 * @param departmentId target inpatient department id
	 * @param deptValid out-parameter: true when the department exists,
	 *        is enabled and is an inpatient department.
	 * @return the ward option list (empty when no enabled ward exists).
	 */
	std::list<InpatientAdmissionWardVO::Wrapper> listWards(
		const std::string& departmentId, bool& deptValid);
};

#endif // !_INPATIENTADMISSIONSERVICE_H_