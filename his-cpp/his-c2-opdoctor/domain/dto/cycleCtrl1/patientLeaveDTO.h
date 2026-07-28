#pragma once
#ifndef _PATIENTLEAVEDTO_H
#define _PATIENTLEAVEDTO_H

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)


class PatientLeaveDTO : public oatpp::DTO
{
    DTO_INIT(PatientLeaveDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, visitStatus, ZH_WORDS_GETTER("leave.dto.newVisitStatus"), false);

    // 唯一标识（请求体必填）
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("leave.dto.encounterId"), true);

};

#include OATPP_CODEGEN_END(DTO)
#endif