#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/18 22:40:00

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
#ifndef _DISPENSESUMMARY_DO_
#define _DISPENSESUMMARY_DO_
#include "../DoInclude.h"

/**
 * Query result entity of the summary bill list (aggregated by summary_no).
 * Not a table entity, so it does not extend BaseDO.
 */
class DispenseSummaryDO
{
	// summary bill no
	MYSQL_SYNTHESIZE(string, summaryNo, SummaryNo);
	// bill status enum
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// destination location id
	MYSQL_SYNTHESIZE(string, desLocationId, DesLocationId);
	// drug item count
	MYSQL_SYNTHESIZE(int, itemCount, ItemCount);
	// planned dispense time
	MYSQL_SYNTHESIZE(string, plannedDispenseTime, PlannedDispenseTime);
	// dispense time
	MYSQL_SYNTHESIZE(string, dispenseTime, DispenseTime);
};

#endif // !_DISPENSESUMMARY_DO_
