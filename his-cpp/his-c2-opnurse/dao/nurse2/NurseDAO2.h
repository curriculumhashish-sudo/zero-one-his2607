/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/07/24

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
#ifndef _NURSE_DAO2_
#define _NURSE_DAO2_
#include "BaseDAO.h"
#include "domain/do/nurse2/nurseDO2.h"
#include "domain/query/nurse2/NurseQuery2.h"

/**
 * Nurse2 data access object
 * Query methods only — insert/update/delete are inherited from BaseDAO
 */
class NurseDAO2 : public BaseDAO
{
private:
	// Build WHERE clause from query filters
	inline std::string queryConditionBuilder(const NurseQuery2::Wrapper& query, SqlParams& params);

public:
	// Count records by query filters
	uint64_t count(const NurseQuery2::Wrapper& query);
	// Select with pagination
	std::list<NurseDO2> selectWithPage(const NurseQuery2::Wrapper& query);
};

#endif // !_NURSE_DAO2_
