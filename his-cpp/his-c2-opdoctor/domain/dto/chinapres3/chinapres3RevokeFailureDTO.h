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
#ifndef _chinapres3RevokeFailureDTO_H_
#define _chinapres3RevokeFailureDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Revoke failure item DTO.
 *
 * Describes a single prescription that could not be revoked,
 * together with a human-readable reason.
 */
class chinapres3RevokeFailureDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3RevokeFailureDTO, DTO);

    // The prescription number that failed to revoke
    API_DTO_FIELD_REQUIRE(
        String,
        prescriptionNo,
        ZH_WORDS_GETTER("chinapres3.field.prescriptionNo"),
        true);

    // Human-readable reason why revocation was blocked
    API_DTO_FIELD_REQUIRE(
        String,
        reason,
        ZH_WORDS_GETTER("chinapres3.field.reason"),
        true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3RevokeFailureDTO_H_
