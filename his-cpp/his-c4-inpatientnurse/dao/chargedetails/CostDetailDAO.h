/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#ifndef _COST_DETAIL_DAO_H_
#define _COST_DETAIL_DAO_H_

#include "BaseDAO.h"
#include "domain/query/chargedetails/CostDetailQuery.h"
#include "domain/query/chargedetails/CostSummaryQuery.h"
#include "domain/dto/chargedetails/CostDetailDTO.h"
#include "domain/dto/chargedetails/CostSummaryDTO.h"

class CostDetailDAO : public BaseDAO
{
private:
	std::string queryConditionBuilder(const CostDetailQuery::Wrapper& query, SqlParams& params);
public:
	uint64_t count(const CostDetailQuery::Wrapper& query);
	std::list<CostDetailDTO::Wrapper> selectWithPage(const CostDetailQuery::Wrapper& query);
	std::list<CostSummaryDTO::Wrapper> selectSummary(const CostSummaryQuery::Wrapper& query);
};

#endif // !_COST_DETAIL_DAO_H_
