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
#ifndef _GET_ITEM_QUERY_
#define _GET_ITEM_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// 获取退费项目查询参数
// 请求体示例: {"encounterId":"337114344591986688","detailViewCode":"PENDING"}
class GetItemQuery : public PageQuery
{
    DTO_INIT(GetItemQuery, PageQuery);

    // 就诊ID（必填）
    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.encounterId");
    };

    // 视图类型编码: PENDING-待退费 / SETTLED-已退费
    DTO_FIELD(String, detailViewCode);
    DTO_FIELD_INFO(detailViewCode) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.detailViewCode");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_GET_ITEM_QUERY_
