#pragma once

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientDetailsDTO : public oatpp::DTO
{
    DTO_INIT(GetPatientDetailsDTO, DTO);

    API_DTO_FIELD(String, hospitalized_number, 
        ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number"),
            true, "ZY2026-07-200001");

    API_DTO_FIELD(String, name,
        ZH_WORDS_GETTER("byj.charging.pay_manage.name"),
        true, "Alice");

    API_DTO_FIELD(String, depart,
        ZH_WORDS_GETTER("byj.charging.pay_manage.depart"),
        true, "Inpatient Internal Medicine");

    API_DTO_FIELD(String, Ward,
        ZH_WORDS_GETTER("byj.charging.pay_manage.Ward"),
        true, "Internal Medicine Ward");

    API_DTO_FIELD(String, bed_number,
        ZH_WORDS_GETTER("byj.charging.pay_manage.bed_number"),
        true, "no 1");

    API_DTO_FIELD(Float64, rest,
        ZH_WORDS_GETTER("byj.charging.pay_manage.rest"),
        true, 100.00);

    API_DTO_FIELD(String, VisitStatus,
        ZH_WORDS_GETTER("byj.charging.pay_manage.VisitStatus"),
        true, "Admitted to Ward");

    API_DTO_FIELD(String, cost_attr,
        ZH_WORDS_GETTER("byj.charging.pay_manage.cost_attr"),
        true, "self-pay");
};

#include OATPP_CODEGEN_END(DTO)
