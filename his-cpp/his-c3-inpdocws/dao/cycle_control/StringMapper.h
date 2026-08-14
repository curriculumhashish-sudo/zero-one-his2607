#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:21:55

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
#ifndef _STRING_MAPPER_H_
#define _STRING_MAPPER_H_

#include "Mapper.h"
#include <string>
using namespace std;

/**
 * 单列字符串查询结果映射器，取第1列值。
 */
class StringMapper : public Mapper<string>
{
public:
	string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(1);
	}
};

#endif // !_STRING_MAPPER_H_
