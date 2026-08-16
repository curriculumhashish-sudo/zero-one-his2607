#pragma once

#include "../../GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(DTO)

// 取消停嘱请求DTO。

// 每条取消停嘱目标都带上实际请求表名。
class CancelStopAdviceItem : public oatpp::DTO {
    DTO_INIT(CancelStopAdviceItem, DTO);
    API_DTO_FIELD_REQUIRE(String, productTableName, ZH_WORDS_GETTER("advice.field.productTableName"), true);
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("advice.field.requestId"), true);
};

class CancelStopAdviceDTO : public oatpp::DTO {
    DTO_INIT(CancelStopAdviceDTO, DTO);
	// 三个范围字段共同确定本次操作的医嘱归属。
	// 租户ID。
    API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("advice.field.tenantId"), true);
	// 就诊ID。
    API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("advice.field.encounterId"), true);
	// 患者ID。
    API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("advice.field.patientId"), true);
	// 取消停嘱目标列表，要求目标医嘱当前处于STOPPED状态。
    API_DTO_FIELD_REQUIRE(List<CancelStopAdviceItem::Wrapper>, lines, ZH_WORDS_GETTER("advice.field.cancelStopLines"), true);
};

#include OATPP_CODEGEN_END(DTO)
