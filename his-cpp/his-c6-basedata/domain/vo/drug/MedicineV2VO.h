#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/08/03

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
#ifndef _MEDICINE_V2_VO_H_
#define _MEDICINE_V2_VO_H_

#include "../../dto/drug/MedicineV2DTO.h"
#include "domain/vo/JsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/** 单项写操作响应。 */
class MedicineVersionJsonVO : public JsonVO<MedicineVersionItemDTO::Wrapper>
{
    DTO_INIT(MedicineVersionJsonVO, JsonVO<MedicineVersionItemDTO::Wrapper>);

  public:
    static Wrapper createExample(int code)
    {
        auto vo = createShared();
        MedicineVersionItemDTO::Wrapper data = MedicineVersionItemDTO::createShared();
        if (code == MedicineV2BusinessCode::SUCCESS)
        {
            data = MedicineVersionItemDTO::createExample("10001", 4);
        }
        vo->init(data, ResultStatus(exampleMessage(code), code));
        return vo;
    }

  private:
    static std::string exampleMessage(int code)
    {
        if (code == MedicineV2BusinessCode::VERSION_CONFLICT)
        {
            return ZH_WORDS_GETTER("drug.v2.message.versionConflict");
        }
        if (code == MedicineV2BusinessCode::PARAM_INVALID)
        {
            return ZH_WORDS_GETTER("drug.v2.message.paramInvalid");
        }
        if (code == MedicineV2BusinessCode::UNAUTHORIZED)
        {
            return ZH_WORDS_GETTER("drug.v2.message.unauthorized");
        }
        if (code == MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED)
        {
            return ZH_WORDS_GETTER("drug.v2.message.businessFailed");
        }
        return ZH_WORDS_GETTER("drug.v2.message.success");
    }
};

/** 批量写操作响应。 */
class MedicineBatchJsonVO : public JsonVO<MedicineBatchResultDTO::Wrapper>
{
    DTO_INIT(MedicineBatchJsonVO, JsonVO<MedicineBatchResultDTO::Wrapper>);

  public:
    static Wrapper createExample(int code)
    {
        auto result = MedicineBatchResultDTO::createEmpty();
        if (code == MedicineV2BusinessCode::PARAM_INVALID ||
            code == MedicineV2BusinessCode::UNAUTHORIZED)
        {
            auto vo = createShared();
            vo->init(result, ResultStatus(exampleMessage(code), code));
            return vo;
        }
        if (code == MedicineV2BusinessCode::SUCCESS || code == MedicineV2BusinessCode::PARTIAL_SUCCESS)
        {
            result->successItems->push_back(MedicineVersionItemDTO::createExample("10001", 4));
        }
        if (code != MedicineV2BusinessCode::SUCCESS)
        {
            auto failed = MedicineFailedItemDTO::createShared();
            failed->id = "10002";
            failed->code = code == MedicineV2BusinessCode::PARTIAL_SUCCESS
                               ? MedicineV2BusinessCode::VERSION_CONFLICT
                               : code;
            failed->message = code == MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED
                                  ? ZH_WORDS_GETTER("drug.v2.message.businessFailed")
                                  : ZH_WORDS_GETTER("drug.v2.message.versionConflict");
            failed->currentVersion = 3;
            result->failedItems->push_back(failed);
        }

        auto vo = createShared();
        vo->init(result, ResultStatus(exampleMessage(code), code));
        return vo;
    }

  private:
    static std::string exampleMessage(int code)
    {
        if (code == MedicineV2BusinessCode::PARTIAL_SUCCESS)
        {
            return ZH_WORDS_GETTER("drug.v2.message.partialSuccess");
        }
        if (code == MedicineV2BusinessCode::VERSION_CONFLICT)
        {
            return ZH_WORDS_GETTER("drug.v2.message.versionConflict");
        }
        if (code == MedicineV2BusinessCode::PARAM_INVALID)
        {
            return ZH_WORDS_GETTER("drug.v2.message.paramInvalid");
        }
        if (code == MedicineV2BusinessCode::UNAUTHORIZED)
        {
            return ZH_WORDS_GETTER("drug.v2.message.unauthorized");
        }
        if (code == MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED)
        {
            return ZH_WORDS_GETTER("drug.v2.message.businessFailed");
        }
        return ZH_WORDS_GETTER("drug.v2.message.success");
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICINE_V2_VO_H_
