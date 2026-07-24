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
#ifndef _NURSE_MAPPER2_
#define _NURSE_MAPPER2_
#include "Mapper.h"
#include "domain/do/nurse2/nurseDO2.h"

/**
 * ResultSet to NurseDO2 mapper for wkf_service_request table
 */
class NurseMapper2 : public Mapper<NurseDO2>
{
public:
	NurseDO2 mapper(ResultSet* resultSet) const override
	{
		NurseDO2 data;
		data.setId(resultSet->getString("id"));
		data.setName(resultSet->getString("bus_no"));
		data.setState(resultSet->getString("status_enum"));
		data.setContent(resultSet->getString("content_json"));
		data.setAmount(resultSet->getInt("execution_count"));
		data.setPerson(resultSet->getString("performer_id"));
		data.setOperate(resultSet->getString("category_enum"));
		return data;
	}
};

#endif // !_NURSE_MAPPER2_
