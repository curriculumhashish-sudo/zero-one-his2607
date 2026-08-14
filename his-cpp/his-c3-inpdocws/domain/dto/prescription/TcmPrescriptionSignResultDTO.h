/*
 Copyright Zero One Star. All rights reserved.

 Defines response data returned after signing TCM prescriptions.

 @Author: nick1484065161
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
#pragma once

// Prevents duplicate DTO declarations when this header is included repeatedly.
#ifndef _TCM_PRESCRIPTION_SIGN_RESULT_DTO_H_
#define _TCM_PRESCRIPTION_SIGN_RESULT_DTO_H_

#include "../../GlobalInclude.h"

// Starts Oat++ DTO code generation.
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Define the traditional Chinese medicine prescription signing result.
 */
class TcmPrescriptionSignResultDTO : public oatpp::DTO
{
    DTO_INIT(TcmPrescriptionSignResultDTO, DTO);

    // Indicate whether the prescription signing operation succeeded.
    API_DTO_FIELD(Boolean, success,
        ZH_WORDS_GETTER("tcm-prescription.field.success"), true, false);

    // Map each prescription group key to its generated prescription number.
    API_DTO_FIELD(Fields<String>, prescriptionNoByGroupKey,
        ZH_WORDS_GETTER("tcm-prescription.field.prescription-no-by-group-key"),
        true, {});
};

// Ends Oat++ DTO code generation.
#include OATPP_CODEGEN_END(DTO)

// Ends the signing result DTO include guard.
#endif // !_TCM_PRESCRIPTION_SIGN_RESULT_DTO_H_
