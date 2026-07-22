#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

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
#ifndef _GET_ITEM_CONTROLLER_
#define _GET_ITEM_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/refund/GetItemQuery.h"
#include "domain/dto/refund/GetItemDTO.h"
#include "domain/vo/refund/GetItemVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

// API 标签 — Swagger 文档分组: "退费管理"
#define API_TAG ZH_WORDS_GETTER("charging.refund.tag")

class GetItemController : public oatpp::web::server::api::ApiController
{
    // JWT 鉴权 — 自动从 Header 提取 token、校验、注入 payload
    API_ACCESS_DECLARE(GetItemController);

public:
    // 端点元数据定义 — Swagger 文档
    // POST /charging/refund/get-item-list — 根据就诊ID查询退费项目明细（嵌套结构）
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("xbzml.charging.refund.get-item-list.summary"),
        queryAll,
        GetItemQuery,
        GetItemJsonVO::Wrapper,
        API_TAG);

    // 端点路由绑定
    // GET 方法，请求体反序列化为 GetItemQuery
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/charging/refund/get-item-list",
        queryAll,
        GetItemQuery,
        execQueryAll(query));

private:
    // 执行查询 — 委托 Service 层处理
    GetItemJsonVO::Wrapper execQueryAll(
        const GetItemQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_GET_ITEM_CONTROLLER_
