#pragma once

#include "../../GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(DTO)

// 每条签发目标都带上实际请求表名，药品医嘱和服务医嘱共用这套结构。
class SignAdviceItem :public oatpp::DTO {
	DTO_INIT(SignAdviceItem, DTO);
	API_DTO_FIELD_REQUIRE(String, productTableName, ZH_WORDS_GETTER("advice.field.productTableName"), true);
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("advice.field.requestId"), true);

};

class SignAdviceDTO :public oatpp::DTO {
	DTO_INIT(SignAdviceDTO, DTO);
	// 三个范围字段共同确定本次操作的医嘱归属。
	API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("advice.field.tenantId"), true);
	API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("advice.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("advice.field.patientId"), true);
	API_DTO_FIELD_REQUIRE(List<SignAdviceItem::Wrapper>, signTargets, ZH_WORDS_GETTER("advice.field.signTargets"), true);
	API_DTO_FIELD_DEFAULT(String, signBatchCode, ZH_WORDS_GETTER("advice.field.signBatchCode"));
};

#include OATPP_CODEGEN_END(DTO)
