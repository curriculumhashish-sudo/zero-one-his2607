/*
 Copyright Zero One Star. All rights reserved.

 签发中药处方响应 VO

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

#ifndef _TCM_PRESCRIPTION_SIGN_VO_H_
#define _TCM_PRESCRIPTION_SIGN_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/TcmPrescriptionSignResultDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Define the JSON response for traditional Chinese medicine prescription signing.
 */
class TcmPrescriptionSignJsonVO: public JsonVO<TcmPrescriptionSignResultDTO::Wrapper>
{
    DTO_INIT(TcmPrescriptionSignJsonVO,JsonVO<TcmPrescriptionSignResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TCM_PRESCRIPTION_SIGN_VO_H_
