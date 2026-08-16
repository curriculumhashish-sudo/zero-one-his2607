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
#ifndef _INPATIENT_ADMISSION_FORM_OPTIONS_DTO_H_
#define _INPATIENT_ADMISSION_FORM_OPTIONS_DTO_H_

#include "../../GlobalInclude.h"
#include "OptionItemDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Aggregated response DTO for the inpatient admission form options endpoint
class InpatientAdmissionFormOptionsDTO : public oatpp::DTO
{
    DTO_INIT(InpatientAdmissionFormOptionsDTO, DTO);

    // Department options for target admission department
    API_DTO_FIELD_REQUIRE(List<OptionItemDTO::Wrapper>, departmentOptions,
        ZH_WORDS_GETTER("doctor2.field.department_options"), true);
    // Admission type options (e.g. elective, emergency)
    API_DTO_FIELD_REQUIRE(List<OptionItemDTO::Wrapper>, admissionTypeOptions,
        ZH_WORDS_GETTER("doctor2.field.admission_type_options"), true);
    // Admission way options (e.g. outpatient transfer, emergency transfer)
    API_DTO_FIELD_REQUIRE(List<OptionItemDTO::Wrapper>, admissionWayOptions,
        ZH_WORDS_GETTER("doctor2.field.admission_way_options"), true);
    // Priority options (e.g. normal, urgent, critical)
    API_DTO_FIELD_REQUIRE(List<OptionItemDTO::Wrapper>, priorityOptions,
        ZH_WORDS_GETTER("doctor2.field.priority_options"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INPATIENT_ADMISSION_FORM_OPTIONS_DTO_H_
