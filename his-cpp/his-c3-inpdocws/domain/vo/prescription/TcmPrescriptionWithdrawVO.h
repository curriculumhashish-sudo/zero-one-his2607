/*
 Copyright Zero One Star. All rights reserved.

 撤回中药处方响应 VO

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

#ifndef _TCM_PRESCRIPTION_WITHDRAW_VO_H_
#define _TCM_PRESCRIPTION_WITHDRAW_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/TcmPrescriptionWithdrawResultDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class TcmPrescriptionWithdrawJsonVO : public JsonVO<TcmPrescriptionWithdrawResultDTO::Wrapper>
{
    DTO_INIT(TcmPrescriptionWithdrawJsonVO, JsonVO<TcmPrescriptionWithdrawResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TCM_PRESCRIPTION_WITHDRAW_VO_H_
