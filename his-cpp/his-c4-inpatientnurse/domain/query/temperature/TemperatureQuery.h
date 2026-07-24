#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 10:00:00

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
#ifndef _TEMPERATURE_QUERY_
    #define _TEMPERATURE_QUERY_
    #include "../../GlobalInclude.h"

    #include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 体温单按日期查询参数。
 * 查询单个患者在单一日期的所有时间段数据。
 * 直接继承 oatpp::DTO（无分页——每次请求返回一张体温单）。
 */
class TemperatureQuery : public oatpp::DTO
{
    DTO_INIT(TemperatureQuery, DTO);
    // 患者ID
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("temperature.field.patientId"), false);
    // 就诊ID
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("temperature.field.encounterId"), false);
    // 记录日期（yyyy-MM-dd）
    API_DTO_FIELD_REQUIRE(String, recordedDate, ZH_WORDS_GETTER("temperature.field.recordedDate"), false);
};

    #include OATPP_CODEGEN_END(DTO)
#endif  // !_TEMPERATURE_QUERY_