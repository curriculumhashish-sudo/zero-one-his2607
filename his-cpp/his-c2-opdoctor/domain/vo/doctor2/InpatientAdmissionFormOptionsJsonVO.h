/*
 Copyright Zero One Star. All rights reserved.

 @Author: fa-mian-tuan
 @Date: 2026/08/09

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
#ifndef _INPATIENT_ADMISSION_FORM_OPTIONS_JSON_VO_H_
#define _INPATIENT_ADMISSION_FORM_OPTIONS_JSON_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/doctor2/InpatientAdmissionFormOptionsDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// JsonVO wrapper for the inpatient admission form options response
class InpatientAdmissionFormOptionsJsonVO : public JsonVO<InpatientAdmissionFormOptionsDTO::Wrapper>
{
    DTO_INIT(InpatientAdmissionFormOptionsJsonVO, JsonVO<InpatientAdmissionFormOptionsDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INPATIENT_ADMISSION_FORM_OPTIONS_JSON_VO_H_
