#pragma once
#ifndef _RECEPTIONACCEPTQUERY_H_
#define _RECEPTIONACCEPTQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RecePatientAcceptQuery : public oatpp::DTO
{
    DTO_INIT(RecePatientAcceptQuery, DTO);

    // 传入的就诊记录 ID（adm_encounter.id）
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("rece.accept.encounterId"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_RECEPTIONACCEPTQUERY_H_