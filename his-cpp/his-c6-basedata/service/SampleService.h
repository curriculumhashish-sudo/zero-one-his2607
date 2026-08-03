#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Su Can
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
#ifndef _SAMPLE_SERVICE_H_
#define _SAMPLE_SERVICE_H_

#include "dao/SampleDAO.h"
#include "domain/dto/Sample/SampleAddDTO.h"
#include "domain/dto/Sample/SampleDTO.h"
#include "domain/dto/Sample/SampleEditDTO.h"
#include "domain/dto/Sample/SampleStatusDTO.h"
#include "domain/query/Sample/SampleQuery.h"
#include "domain/query/Sample/SampleNameQuery.h"
#include <list>
#include <string>

class SampleService
{
public:
	std::list<SampleNameDTO::Wrapper> listSampleNames(const SampleNameQuery::Wrapper& query) const;
	SamplePageDTO::Wrapper listSamplePage(const SampleQuery::Wrapper& query) const;
	void addSample(const SampleAddDTO::Wrapper& dto) const;
	void editSample(const SampleEditDTO::Wrapper& dto) const;
	void removeSample(const std::string& id) const;
	void switchSampleStatus(const SampleStatusDTO::Wrapper& dto) const;

private:
	inline static constexpr const char* DEFAULT_OPERATOR = "system";
	inline static constexpr const char* DEFAULT_SAMPLE_TYPE = "BLOOD";

	static SampleDTO::Wrapper toSampleDTO(const PtrSampleDO& sample);
	static oatpp::String firstPresent(const oatpp::String& primary, const oatpp::String& fallback);
	static std::string resolveStatusEnum(const oatpp::String& statusEnum, const oatpp::Int32& status, int defaultStatus = 0);
	std::string buildSampleId() const;
	std::string currentTime() const;
	static std::string toStdString(const oatpp::String& value);
};

#endif // !_SAMPLE_SERVICE_H_
