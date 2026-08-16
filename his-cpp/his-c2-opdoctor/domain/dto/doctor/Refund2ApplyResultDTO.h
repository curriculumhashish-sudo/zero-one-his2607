#pragma once
#ifndef __REFUNDAPPLYRESULTDTO_H_
#define __REFUNDAPPLYRESULTDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Refund2ApplyResultDTO :public oatpp::DTO
{
    DTO_INIT(Refund2ApplyResultDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, applicationId, ZH_WORDS_GETTER("cyclectrl2.refund.applicationId"));
    API_DTO_FIELD_DEFAULT(String, applicationNo, ZH_WORDS_GETTER("cyclectrl2.refund.applicationNo"));
    API_DTO_FIELD_DEFAULT(String, status, ZH_WORDS_GETTER("cyclectrl2.refund.status"));
    API_DTO_FIELD_DEFAULT(String, statusName, ZH_WORDS_GETTER("cyclectrl2.refund.statusName"));
    API_DTO_FIELD_DEFAULT(Float64, refundAmount, ZH_WORDS_GETTER("cyclectrl2.refund.refundAmount"));
    API_DTO_FIELD_DEFAULT(String, createdAt, ZH_WORDS_GETTER("cyclectrl2.refund.createdAt"));

};

#include OATPP_CODEGEN_END(DTO)

#endif
