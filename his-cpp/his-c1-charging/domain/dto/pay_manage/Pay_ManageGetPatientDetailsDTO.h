#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: byj
 @Date: 2026/08/02

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
#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientDetailsDTO : public oatpp::DTO
{
    DTO_INIT(Pay_ManageGetPatientDetailsDTO, DTO);

    API_DTO_FIELD(String, hospitalized_number,
        ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number"),
            true, "ZY2026-07-200001");

    API_DTO_FIELD(String, name,
        ZH_WORDS_GETTER("byj.charging.pay_manage.name"),
        true, "Alice");

    API_DTO_FIELD(String, depart,
        ZH_WORDS_GETTER("byj.charging.pay_manage.depart"),
        true, "Inpatient Internal Medicine");

    API_DTO_FIELD(String, ward,
        ZH_WORDS_GETTER("byj.charging.pay_manage.ward"),
        true, "Internal Medicine Ward");

    API_DTO_FIELD(String, bed_number,
        ZH_WORDS_GETTER("byj.charging.pay_manage.bed_number"),
        true, "no 1");

    API_DTO_FIELD(Float64, rest,
        ZH_WORDS_GETTER("byj.charging.pay_manage.rest"),
        true, 100.00);

    API_DTO_FIELD(String, visitStatus,
        ZH_WORDS_GETTER("byj.charging.pay_manage.visit_status"),
        true, "Admitted to Ward");

    API_DTO_FIELD(String, cost_attr,
        ZH_WORDS_GETTER("byj.charging.pay_manage.cost_attr"),
        true, "self-pay");
};

#include OATPP_CODEGEN_END(DTO)
