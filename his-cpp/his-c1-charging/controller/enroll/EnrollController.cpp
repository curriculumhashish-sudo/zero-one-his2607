/*
 Copyright Zero One Star. All rights reserved.

 @Author: QQQQ
 @Date: 2026/07/19

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

#include "stdafx.h"
#include "EnrollController.h"

/**
 * @brief 执行住院登记分页列表接口。
 *
 * 接收 Controller 解析后的查询条件，调用 EnrollService 查询真实数据库数据，
 * 再将分页 DTO 包装为统一 JSON 响应对象。
 *
 * @param query 前端传入的住院登记查询条件。
 * @return 包含分页住院登记数据的统一 JSON 响应对象。
 */
EnrollPageJsonVO::Wrapper EnrollController::execQueryAll(
    const EnrollQuery::Wrapper& query
)
{
    // Controller 只协调接口调用，不直接编写 SQL 或转换数据库对象。
    EnrollService service;

    // Service 返回已组装完成的分页 DTO。
    auto page = service.queryAll(query);

    // 使用项目统一响应结构包装数据。
    auto result = EnrollPageJsonVO::createShared();
    result->init(page, ResultStatus(ZH_WORDS_GETTER("QQQQ.message.success")));

    return result;
}

/**
 * @brief 根据住院登记主键查询一条真实的住院登记详情。
 *
 * Controller 只负责校验接口参数、调用 Service 并包装统一响应；
 * 具体的数据库查询、业务字段转换均由下层完成。
 *
 * @param encounterId 前端传入的住院登记主键，对应 adm_encounter.id。
 * @return 成功时返回详情数据；参数为空或记录不存在时返回相应失败状态。
 *
 * @details
 * 这里不能再构造演示数据，否则 Swagger 看起来成功，
 * 但页面拿到的并不是数据库中的真实住院登记。
 */
EnrollDetailJsonVO::Wrapper EnrollController::execQueryById(const String& encounterId)
{
    auto result = EnrollDetailJsonVO::createShared();

    if (!encounterId || encounterId->empty())
    {
        result->init(nullptr, RS_PARAMS_INVALID);
        return result;
    }

    EnrollService service;
    auto detail = service.queryById(*encounterId);

    if (!detail)
    {
        result->init(nullptr, RS_FAIL);
        return result;
    }

    result->init(detail, ResultStatus(ZH_WORDS_GETTER("QQQQ.message.success")));
    return result;
}
