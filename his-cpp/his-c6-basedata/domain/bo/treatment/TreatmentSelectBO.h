#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/30

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
#ifndef _TREATMENT_SELECT_BO_H_
#define _TREATMENT_SELECT_BO_H_

#include "Macros.h"
#include "oatpp/core/Types.hpp"

#include <cstdint>
#include <string>

/** 选择诊疗联表查询结果。 */
class TreatmentSelectBO
{
	CC_SYNTHESIZE(std::string, id, Id);
	CC_SYNTHESIZE(std::string, busNo, BusNo);
	CC_SYNTHESIZE(std::string, name, Name);
	CC_SYNTHESIZE(std::string, categoryCode, CategoryCode);
	CC_SYNTHESIZE(std::string, categoryCodeDictText, CategoryCodeDictText);
	CC_SYNTHESIZE(std::string, packageUnitCode, PackageUnitCode);
	CC_SYNTHESIZE(std::string, packageUnitCodeDictText, PackageUnitCodeDictText);
	CC_SYNTHESIZE(std::string, defEncounterUnitCode, DefEncounterUnitCode);
	CC_SYNTHESIZE(std::string, defEncounterUnitCodeDictText, DefEncounterUnitCodeDictText);
	CC_SYNTHESIZE(int32_t, version, Version);
};

#endif // !_TREATMENT_SELECT_BO_H_
