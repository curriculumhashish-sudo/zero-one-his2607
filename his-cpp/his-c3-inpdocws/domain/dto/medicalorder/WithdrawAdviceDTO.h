#pragma once

#include "../../GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(DTO)

/*
*取消医嘱DTO类
*/

class WithdrawAdviceItem : public oatpp::DTO {
	DTO_INIT(WithdrawAdviceItem, DTO);
    //产品定义表名
	API_DTO_FIELD_REQUIRE(String, productTableName, ZH_WORDS_GETTER("advice.field.productTableName"), true);
    //请求ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("advice.field.requestId"), true);
};

class WithdrawAdviceDTO : public oatpp::DTO {
    DTO_INIT(WithdrawAdviceDTO, DTO);
    //就诊ID
    API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("advice.field.encounterId"), true);
    //患者ID
    API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("advice.field.patientId"), true);
    //产品定义组
    API_DTO_FIELD_REQUIRE(List<WithdrawAdviceItem::Wrapper>, targets, ZH_WORDS_GETTER("advice.field.withdrawTargets"), true);
};

#include OATPP_CODEGEN_END(DTO)