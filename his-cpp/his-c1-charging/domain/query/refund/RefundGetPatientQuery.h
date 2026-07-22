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
#ifndef _GET_PATIENT_QUERY_
#define _GET_PATIENT_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientQuery : public PageQuery
{
    DTO_INIT(GetPatientQuery, PageQuery);

    // Keyword for fuzzy matching (patient name, encounter ID, etc.)
    DTO_FIELD(String, keyword);
    DTO_FIELD_INFO(keyword) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.keyword");
    };

    // Queue tab filter: ALL / PENDING / SETTLED
    DTO_FIELD(String, queueTabCode);
    DTO_FIELD_INFO(queueTabCode) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.queueTabCode");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_GET_PATIENT_QUERY_
