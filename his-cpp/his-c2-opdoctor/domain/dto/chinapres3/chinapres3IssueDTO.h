#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2025/07/26

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
#ifndef _chinapres3ISSUEDTO_H_
#define _chinapres3ISSUEDTO_H_

#include "../../GlobalInclude.h"
#include "chinapres3SignTargetDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 处方开立请求传输对象
 *
 * 承载中药处方开立操作所需全部入参。
 * 单次开立请求可处理多条待签署目标（处方单据记录），
 * 每条目标由 chinapres3SignTargetDTO 承载标识信息。
 *
 * @see chinapres3SignTargetDTO
 */
class chinapres3IssueDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3IssueDTO, DTO);


    //这两个用来确定边界 判断这个数据是本次就诊同时判断是该患者
    // 就诊ID，用于标识本次开立操作对应的就诊上下文
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("chinapres3.field.encounterId"), true);

    // 患者ID，用于标识就诊患者
    API_DTO_FIELD_REQUIRE(String, patientId, ZH_WORDS_GETTER("chinapres3.field.patientId"), true);



    // 待开立处方目标集合，每一项代表一条需要开立的中药处方
    //OBject是oatpp里面的写法
    API_DTO_FIELD_REQUIRE(Vector<Object<chinapres3SignTargetDTO>>, signTargets, ZH_WORDS_GETTER("chinapres3.field.signTargets"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3ISSUEDTO_H_