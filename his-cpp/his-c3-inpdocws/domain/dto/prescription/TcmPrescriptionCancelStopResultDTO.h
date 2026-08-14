/*
 Copyright Zero One Star. All rights reserved.

 Traditional Chinese medicine cancel-stop result DTO

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

// Prevents this header from being included more than once.
#ifndef _TCM_PRESCRIPTION_CANCEL_STOP_RESULT_DTO_H_
#define _TCM_PRESCRIPTION_CANCEL_STOP_RESULT_DTO_H_

// Provides project-wide definitions and DTO macros.
#include "../../GlobalInclude.h"

// Begins Oatpp DTO code generation.
#include OATPP_CODEGEN_BEGIN(DTO)

// Carries the business result of a cancel-stop operation.
class TcmPrescriptionCancelStopResultDTO : public oatpp::DTO
{
    // Initializes Oatpp metadata for this DTO type.
    DTO_INIT(TcmPrescriptionCancelStopResultDTO, DTO);

    // Indicates whether the cancel-stop operation succeeded.
    API_DTO_FIELD(Boolean, success, ZH_WORDS_GETTER("tcm-prescription.field.success"), true, true);

    // Describes the outcome of the cancel-stop operation.
    API_DTO_FIELD(String, message, ZH_WORDS_GETTER("tcm-prescription.field.message"), true, "operation completed");
};

// Ends Oatpp DTO code generation.
#include OATPP_CODEGEN_END(DTO)

// Ends the header guard.
#endif // _TCM_PRESCRIPTION_CANCEL_STOP_RESULT_DTO_H_
