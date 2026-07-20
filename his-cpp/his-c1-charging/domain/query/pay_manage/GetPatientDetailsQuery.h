#pragma once

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientDetailsQuery : public oatpp::DTO {
    DTO_INIT(GetPatientDetailsQuery, DTO);

    API_DTO_FIELD(String, hospitalized_number,
        ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number"),
        true, "ZY2026-07-200001");
};

#include OATPP_CODEGEN_END(DTO)