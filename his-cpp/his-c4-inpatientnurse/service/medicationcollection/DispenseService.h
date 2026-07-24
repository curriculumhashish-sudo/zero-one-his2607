#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
 @Date: 2026/07/18 22:50:00

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
#ifndef _DISPENSE_SERVICE_
#define _DISPENSE_SERVICE_

#include "domain/query/medicationcollection/DispenseSummaryQuery.h"
#include "domain/dto/medicationcollection/DispenseDTO.h"

/**
 * Business service of the summary drug-collection feature.
 */
class DispenseService
{
public:
	// query summary bill list with pagination (flow: validate -> cache -> db -> assemble)
	DispenseSummaryPageDTO::Wrapper listSummary(const DispenseSummaryQuery::Wrapper& query);
	// query dispense details by summary bill no
	oatpp::List<oatpp::Object<DispenseDetailDTO>> listDetail(const std::string& summaryNo);
};
#endif // !_DISPENSE_SERVICE_
