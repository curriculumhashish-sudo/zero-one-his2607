#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: 苏灿
 @Date: 2026/07/25 10:00:00

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
#ifndef _SAMPLE_DAO_H_
#define _SAMPLE_DAO_H_

#include "BaseDAO.h"
#include "domain/do/Sample/SampleDO.h"
#include <list>
#include <string>

class SampleDAO : public BaseDAO
{
public:
	std::list<PtrSampleDO> selectByKeyword(const std::string& keyword, uint64_t offset, uint64_t limit);
	uint64_t countByKeyword(const std::string& keyword);
	PtrSampleDO selectById(const std::string& id);
	bool existsByCode(const std::string& sampleCode, const std::string& excludeId = "");

	// 新增
	int insertSample(const SampleDO& sample);
	// 修改
	int updateSample(const SampleDO& sample);
	// 删除
	int removeSampleById(const std::string& id);
	// 启停
	int updateStatusByIds(const std::list<std::string>& ids, int status, const std::string& updateBy, const std::string& updateTime);
};

#endif // !_SAMPLE_DAO_H_
