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
#include "stdafx.h"
#include "controller/data_statistics/MonthVisitController.h"
#include"../../service/data_statistics/MonthVisitService.h"


MonthVisitPageJsonVO::Wrapper MonthVisitController::execQueryMonthVisit(
	const MonthVisitQuery::Wrapper& query
)
{
	auto result = MonthVisitService().findflow(query);
	auto jvo = MonthVisitPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}
