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
#ifndef _TEMPERATURECONTROLLER_H_
    #define _TEMPERATURECONTROLLER_H_

    #include "domain/vo/BaseJsonVO.h"
    #include "domain/query/temperature/TemperatureQuery.h"
    #include "domain/dto/temperature/TemperatureDTO.h"
    #include "domain/vo/temperature/TemperatureVO.h"

    #include OATPP_CODEGEN_BEGIN(ApiController)
    #define API_TAG ZH_WORDS_GETTER("temperature.tags.tag")

/**
 * 体温单 API 控制器。
 *
 * 后端范围（2个端点）：
 *  - 按患者 + 日期查询单日体温单
 *  - 保存/更新整张体温单（7个时间段 + 补充记录）
 */
class TemperatureController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(TemperatureController);

public:
    // 按日期查询体温单
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("temperature.query-by-date.summary"),
                                     queryByDate,
                                     TemperatureQuery,
                                     TemperatureJsonVO::Wrapper,
                                     API_TAG);
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET, "/temperature/query-by-date", queryByDate, TemperatureQuery, execQueryByDate(query));

    // 保存体温单（插入或更新）
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("temperature.save.summary"),
                               saveTemperature,
                               StringJsonVO::Wrapper,
                               API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST,
                              "/temperature/save",
                              saveTemperature,
                              BODY_DTO(TemperatureSaveDTO::Wrapper, dto),
                              execSave(dto, authObject->getPayload()));

private:
    TemperatureJsonVO::Wrapper execQueryByDate(const TemperatureQuery::Wrapper& query);
    StringJsonVO::Wrapper execSave(const TemperatureSaveDTO::Wrapper& dto, const PayloadDTO& payload);
};

    #undef API_TAG
    #include OATPP_CODEGEN_END(ApiController)
#endif  // !_TEMPERATURECONTROLLER_H_