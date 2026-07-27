#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
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
#ifndef _TREATMENTQUERY_H_
#define _TREATMENTQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊疗分页查询条件，继承分页基类
 */
//class TreatmentQuery : public PageQuery
//{
//    DTO_INIT(TreatmentQuery, PageQuery);
//    // 关键字，按诊疗名称或编码模糊匹配
//    API_DTO_FIELD_REQUIRE(String, keywords, ZH_WORDS_GETTER("treatment.field.keywords"), false);
//    // 启用/停用状态过滤（1/0），不传表示不过滤
//    API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("treatment.field.status"), false);
//};
// 列表分页用
class TreatmentQuery : public PageQuery
{
    DTO_INIT(TreatmentPageQuery, PageQuery);
    // 关键字，按诊疗名称或编码模糊匹配
    API_DTO_FIELD_REQUIRE(String, keywords, ZH_WORDS_GETTER("treatment.field.keywords"), false);
    // 分类
    API_DTO_FIELD_REQUIRE(String, classification, ZH_WORDS_GETTER("treatment.field.classification"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTQUERY_H_
