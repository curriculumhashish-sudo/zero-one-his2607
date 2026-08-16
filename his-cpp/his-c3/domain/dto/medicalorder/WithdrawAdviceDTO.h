#pragma once

#include "../../GlobalInclude.h"


#include OATPP_CODEGEN_BEGIN(DTO)

// 撤回医嘱请求DTO。

// 每条撤回目标都带上实际请求表名，避免只凭ID猜测医嘱类型。
class WithdrawAdviceItem : public oatpp::DTO {
	DTO_INIT(WithdrawAdviceItem, DTO);
	// 产品请求表名。
	API_DTO_FIELD_REQUIRE(String, productTableName, ZH_WORDS_GETTER("advice.field.productTableName"), true);
	// 医嘱请求ID。
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("advice.field.requestId"), true);
};

class WithdrawAdviceDTO : public oatpp::DTO {
    DTO_INIT(WithdrawAdviceDTO, DTO);
	// 三个范围字段共同确定本次操作的医嘱归属。
	// 租户ID。
    API_DTO_FIELD_REQUIRE(String, tenant_id, ZH_WORDS_GETTER("advice.field.tenantId"), true);
	// 就诊ID。
    API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("advice.field.encounterId"), true);
	// 患者ID。
    API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("advice.field.patientId"), true);
	// 撤回目标列表，要求目标医嘱当前处于ACTIVE状态。
    API_DTO_FIELD_REQUIRE(List<WithdrawAdviceItem::Wrapper>, targets, ZH_WORDS_GETTER("advice.field.withdrawTargets"), true);
};

#include OATPP_CODEGEN_END(DTO)
