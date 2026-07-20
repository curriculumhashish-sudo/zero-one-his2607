#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
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
#ifndef _TREATMENTPAGEDTO_H_
#define _TREATMENTPAGEDTO_H_

#include "../../GlobalInclude.h"
#include "TreatmentDetailDTO.h"
#include "TreatmentSelectDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊疗分页列表 DTO（详细）
 */
class TreatmentPageDTO : public PageDTO<TreatmentDetailDTO::Wrapper>
{
    DTO_INIT(TreatmentPageDTO, PageDTO<TreatmentDetailDTO::Wrapper>);
};

/**
 * 诊疗分页列表 DTO（选择/下拉 简化）
 */
class TreatmentSelectPageDTO : public PageDTO<TreatmentSelectDTO::Wrapper>
{
    DTO_INIT(TreatmentSelectPageDTO, PageDTO<TreatmentSelectDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENTPAGEDTO_H_
