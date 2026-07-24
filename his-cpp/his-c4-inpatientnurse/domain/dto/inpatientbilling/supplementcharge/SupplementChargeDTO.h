#pragma once
//补费请求参数
#ifndef _SUPPLEMENTCHARGEDTO_H_
#define _SUPPLEMENTCHARGEDTO_H_

#include "../../../GlobalInclude.h"
#include "ApiHelper.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SupplementChargeDTO : public oatpp::DTO {

    DTO_INIT(SupplementChargeDTO, DTO);

    //患者住院号
    API_DTO_FIELD(String, inpNo, ZH_WORDS_GETTER("supplementcharge.field.inpNo"), true, "");

    //医嘱ID
    API_DTO_FIELD(String, orderId, ZH_WORDS_GETTER("supplementcharge.field.orderId"), true, "");

    //操作人ID/工号
    API_DTO_FIELD(String, operatorid, ZH_WORDS_GETTER("supplementcharge.field.operatorid"), true, "");

    //补费日期
    API_DTO_FIELD(String, supplementDate, ZH_WORDS_GETTER("supplementcharge.field.supplementDate"), true, "");

    //记账科室编码
    API_DTO_FIELD(String, chargeDept, ZH_WORDS_GETTER("supplementcharge.field.chargeDept"), true, "");

    //补费原因说明
    API_DTO_FIELD(String, supplementReason, ZH_WORDS_GETTER("supplementcharge.field.supplementReason"), false, "");

};

#include OATPP_CODEGEN_END(DTO)

#endif //_SUPPLEMENTCHARGEDTO_H_