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
#ifndef _chinapres3IssueResultVO_H_
#define _chinapres3IssueResultVO_H_

#include "../../GlobalInclude.h"
#include "domain/vo/JsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 开立操作结果数据对象（嵌套在chinapres3IssueResultVO的data字段内）
 *
 * 记录单次处方开立操作的汇总信息：
 *   - 处理成功的处方单据条数
 *   - 参与开立的处方单总数
 *   * 生成的业务流水号集合（发药记录流水号busNos）
 */
class chinapres3IssueResultDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3IssueResultDTO, DTO);

    // 处理成功的中药处方单据数量
    API_DTO_FIELD_REQUIRE(
        Int32,
        processedCount,
        ZH_WORDS_GETTER("chinapres3.field.processedCount"),
        true);

    // 实际处理的处方单总数（数值可小于processedCount，一张处方包含多条药品记录）
    API_DTO_FIELD_REQUIRE(
        Int32,
        prescriptionCount,
        ZH_WORDS_GETTER("chinapres3.field.prescriptionCount"),
        true);

    // 生成的发药业务流水号列表（每条发药记录对应的busNo）
    DTO_FIELD(Vector<String>, busNos);
    DTO_FIELD_INFO(busNos) {
        info->description = ZH_WORDS_GETTER("chinapres3.field.busNos");
    }
};

/**
 * 处方开立返回视图对象
 *
 * 继承通用JSON返回体模板 JsonVO<chinapres3IssueResultDTO::Wrapper>。
 * 接口响应格式：code业务状态码、message提示信息、data开立结果详情。
 */
class chinapres3IssueResultVO : public JsonVO<chinapres3IssueResultDTO::Wrapper>
{
    DTO_INIT(chinapres3IssueResultVO, JsonVO<chinapres3IssueResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3IssueResultVO_H_