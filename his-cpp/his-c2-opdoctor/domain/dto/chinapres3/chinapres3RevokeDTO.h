#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2026/07/27

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
#ifndef _chinapres3RevokeDTO_H_
#define _chinapres3RevokeDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 处方撤回请求DTO
 *
 * 用于传递需要执行撤回操作的处方单号集合
 * 单次撤回接口请求，会撤销传入处方号下所有药品明细行，
 * 并同步作废关联的发药记录、收费结算记录
 */
class chinapres3RevokeDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3RevokeDTO, DTO);

    // 需要撤回的处方单号数组，同一个处方号对应多条同处方的药品申请明细数据
    API_DTO_FIELD_REQUIRE(
        Vector<String>,
        prescriptionNos,
        ZH_WORDS_GETTER("chinapres3.field.prescriptionNos"),
        true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3RevokeDTO_H_