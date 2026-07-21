#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: QQQQ
 @Date: 2026/07/18

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

#include "../../GlobalInclude.h"
#include "../../dto/enroll/EnrollDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Paginated inpatient registration response.
 */
class EnrollPageJsonVO : public JsonVO<EnrollPageDTO::Wrapper>
{
    DTO_INIT(EnrollPageJsonVO, JsonVO<EnrollPageDTO::Wrapper>);
};

/**
 * Inpatient registration detail response.
 */
class EnrollDetailJsonVO : public JsonVO<EnrollDetailDTO::Wrapper>
{
    DTO_INIT(EnrollDetailJsonVO, JsonVO<EnrollDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)