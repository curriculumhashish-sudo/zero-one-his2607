#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/29

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
#ifndef _DIAGNOSE_CONSTANTS_H_
#define _DIAGNOSE_CONSTANTS_H_

#include <array>

/** 选择诊疗查询各层共用的业务常量。 */
class DiagnoseConstants final
{
public:
	// 诊疗类型筛选值及响应值。
	inline static constexpr const char* KIND_FILTER_WESTERN = "1";
	inline static constexpr const char* KIND_FILTER_TCM = "2";
	inline static constexpr const char* KIND_WESTERN = "WESTERN";
	inline static constexpr const char* KIND_TCM = "TCM";

	// 已启用状态及其前端排序值。
	inline static constexpr const char* STATUS_ACTIVE = "ACTIVE";
	inline static constexpr int STATUS_ACTIVE_SORT_ORDER = 2;

	// 西医诊疗来源。
	inline static constexpr std::array<const char*, 6> WESTERN_SOURCE_CODES = {
		"general_dx",
		"chronic_spec",
		"disease_pkg",
		"paid_dx",
		"day_surgery_dx",
		"tumor_morph"
	};

	// 中医诊疗来源。
	inline static constexpr std::array<const char*, 2> TCM_SOURCE_CODES = {
		"tcm_dx",
		"tcm_syndrome"
	};
};

#endif // !_DIAGNOSE_CONSTANTS_H_
