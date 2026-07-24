#pragma once
//补费请求参数
#ifndef _SUPPLEMENTCHARGEQUERY_H_
#define _SUPPLEMENTCHARGEQUERY_H_

#include "../../../GlobalInclude.h"
#include "ApiHelper.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SupplementChargeQuery : public DTO {

    DTO_INIT(SupplementChargeQuery, DTO);

    //患者住院号
    API_DTO_FIELD_REQUIRE(String, inpNo, ZH_WORDS_GETTER("supplementcharge.field.inpNo"), true);

    //医嘱ID
    API_DTO_FIELD_REQUIRE(String, orderId, ZH_WORDS_GETTER("supplementcharge.field.orderId"), true);

    //操作人ID/工号
    API_DTO_FIELD_REQUIRE(String, operatorid, ZH_WORDS_GETTER("supplementcharge.field.operatorid"), true);

    //补费日期
    API_DTO_FIELD_REQUIRE(String, supplementDate, ZH_WORDS_GETTER("supplementcharge.field.supplementDate"), true);

    //记账科室编码
    API_DTO_FIELD_REQUIRE(String, chargeDept, ZH_WORDS_GETTER("supplementcharge.field.chargeDept"), true);

    //补费原因说明
    API_DTO_FIELD_REQUIRE(String, supplementReason, ZH_WORDS_GETTER("supplementcharge.field.supplementReason"), false);

};

#include OATPP_CODEGEN_END(DTO)

#endif //_SUPPLEMENTCHARGEQUERY_H_