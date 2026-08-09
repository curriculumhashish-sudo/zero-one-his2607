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
#include "stdafx.h"
#include "InpatientAdmissionController.h"
#include "../../service/cycleCtrl2/InpatientAdmissionService.h"

InpatientAdmissionWardListJsonVO::Wrapper InpatientAdmissionController::execListInpatientAdmissionWards(
	const InpatientAdmissionWardsQuery::Wrapper& query)
{
	auto jvo = InpatientAdmissionWardListJsonVO::createShared();

	// Required field validation
	if (!query->departmentId || query->departmentId.getValue("").empty())
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	bool deptValid = false;
	auto wards = InpatientAdmissionService().listWards(query->departmentId.getValue(""), deptValid);

	// The target department does not exist, is disabled, or is not an
	// inpatient department: return a business error with code 9999.
	if (!deptValid)
	{
		jvo->init(nullptr, ResultStatus(
			ZH_WORDS_GETTER("cyclectrl2.admission.deptNotFound"), 9999));
		return jvo;
	}

	// Success: the department is valid, return the ward option list.
	// An empty list is also a successful response.
	auto data = oatpp::List<InpatientAdmissionWardVO::Wrapper>::createShared();
	for (const auto& ward : wards)
	{
		data->push_back(ward);
	}
	jvo->success(data);
	return jvo;
}