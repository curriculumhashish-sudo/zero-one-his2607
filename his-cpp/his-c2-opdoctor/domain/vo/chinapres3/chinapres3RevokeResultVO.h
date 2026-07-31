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
#ifndef _chinapres3RevokeResultVO_H_
#define _chinapres3RevokeResultVO_H_

#include "../../GlobalInclude.h"
#include "domain/vo/JsonVO.h"
// ---- revoke-related include ----
// Must be included BEFORE OATPP_CODEGEN_BEGIN(DTO) because
// chinapres3RevokeFailureDTO.h itself opens and closes the DTO codegen block.
// Including it inside the current block would prematurely undefine all DTO macros.
#include "../../dto/chinapres3/chinapres3RevokeFailureDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Revoke result data DTO, nested inside chinapres3RevokeResultVO.data.
 *
 * Reports how many prescriptions / medication lines were successfully
 * revoked, and lists any prescriptions that could not be processed.
 */
class chinapres3RevokeResultDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3RevokeResultDTO, DTO);

    // Number of prescriptions successfully revoked
    API_DTO_FIELD_REQUIRE(
        Int32,
        revokedPrescriptionCount,
        ZH_WORDS_GETTER("chinapres3.field.revokedPrescriptionCount"),
        true);

    // Number of medication-request rows successfully reverted to DRAFT
    API_DTO_FIELD_REQUIRE(
        Int32,
        revokedMedicationCount,
        ZH_WORDS_GETTER("chinapres3.field.revokedMedicationCount"),
        true);

    // Prescriptions that could not be revoked, together with the reason
    DTO_FIELD(Vector<Object<chinapres3RevokeFailureDTO>>, failures);
    DTO_FIELD_INFO(failures)
    {
        info->description = ZH_WORDS_GETTER("chinapres3.field.failures");
    }
};

/**
 * Revoke result view object.
 *
 * Inherits JsonVO<chinapres3RevokeResultDTO::Wrapper> so the HTTP response
 * follows the standard { code, message, data } envelope.
 */
class chinapres3RevokeResultVO : public JsonVO<chinapres3RevokeResultDTO::Wrapper>
{
    DTO_INIT(chinapres3RevokeResultVO, JsonVO<chinapres3RevokeResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3RevokeResultVO_H_
