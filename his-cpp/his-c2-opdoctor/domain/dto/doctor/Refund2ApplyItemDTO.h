#pragma once
#ifndef __REFUNDAPPLYITEMDTO_H_
#define __REFUNDAPPLYITEMDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Refund2ApplyItemDTO :public oatpp::DTO
{
    DTO_INIT(Refund2ApplyItemDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, paymentNo, ZH_WORDS_GETTER("cyclectrl2.patient.paymentNo"));

    API_DTO_FIELD_DEFAULT(Float64, refundQuantity, ZH_WORDS_GETTER("cyclectrl2.patient.quantity"));

};

#include OATPP_CODEGEN_END(DTO)

#endif
