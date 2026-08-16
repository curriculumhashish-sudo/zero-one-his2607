/*
 Copyright Zero One Star. All rights reserved.

 Traditional Chinese medicine prescription-stop response VO

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
#ifndef _TCM_PRESCRIPTION_STOP_VO_H_
#define _TCM_PRESCRIPTION_STOP_VO_H_

// Provides project-wide definitions.
#include "../../GlobalInclude.h"

// Provides the business result DTO for prescription-stop operations.
#include "../../dto/prescription/TcmPrescriptionStopResultDTO.h"

// Begins Oatpp DTO code generation.
#include OATPP_CODEGEN_BEGIN(DTO)

// Wraps a prescription-stop result in the standard JSON response format.
class TcmPrescriptionStopJsonVO
    : public JsonVO<TcmPrescriptionStopResultDTO::Wrapper>
{
    // Initializes Oatpp metadata for this response type.
    DTO_INIT(
        TcmPrescriptionStopJsonVO,
        JsonVO<TcmPrescriptionStopResultDTO::Wrapper>
    );
};

// Ends Oatpp DTO code generation.
#include OATPP_CODEGEN_END(DTO)

// Ends the header guard.
#endif // _TCM_PRESCRIPTION_STOP_VO_H_
