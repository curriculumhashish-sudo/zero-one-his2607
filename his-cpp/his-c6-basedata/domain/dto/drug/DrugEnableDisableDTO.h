#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/23

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
#ifndef _DRUG_ENABLE_DISABLE_DTO_H_
#define _DRUG_ENABLE_DISABLE_DTO_H_

#include "../../GlobalInclude.h"
#include "../../DrugConstants.h"
#include "domain/dto/PayloadDTO.h"

#include <algorithm>
#include <cctype>

#include OATPP_CODEGEN_BEGIN(DTO)

/** 批量启停药品请求。 */
class DrugEnableDisableDTO : public oatpp::DTO
{
    DTO_INIT(DrugEnableDisableDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<String>, ids, ZH_WORDS_GETTER("drug.field.ids"), true);
    API_DTO_FIELD_REQUIRE(Boolean, activeFlag, ZH_WORDS_GETTER("drug.field.activeFlag"), true);

    // 控制器注入登录信息。
    CC_SYNTHESIZE(const PayloadDTO *, _payload, Payload);

  public:
    /** 构造 Swagger 请求示例。 */
    static Wrapper createExample(bool active)
    {
        auto dto = createShared();
        dto->ids = oatpp::List<oatpp::String>::createShared();
        dto->ids->push_back("10001");
        dto->ids->push_back("10002");
        dto->activeFlag = active;
        return dto;
    }

    // 校验目标状态和批次。
    std::string validate() const
    {
        if (activeFlag == nullptr || !ids || ids->empty() || ids->size() > DrugConstants::MAX_BATCH_SIZE)
        {
            return "batch status request invalid.";
        }
        for (const auto &id : *ids.get())
        {
            if (!isValidId(id))
            {
                return "id invalid.";
            }
        }
        return "";
    }

  private:
    static constexpr size_t MAX_ID_LENGTH = 64;

    // 校验编号格式。
    static bool isValidId(const oatpp::String &id)
    {
        if (!id)
        {
            return false;
        }
        const std::string value = id.getValue("");
        return !value.empty() && value.size() <= MAX_ID_LENGTH &&
               std::all_of(value.cbegin(), value.cend(), [](unsigned char character)
                           { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DRUG_ENABLE_DISABLE_DTO_H_
