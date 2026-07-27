#pragma once
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 处方目标DTO
 * 存储处方标识相关信息，用于同步签署处方数据
 */
class ChinaPres3SignTargetDTO : public oatpp::DTO
{
    DTO_INIT(ChinaPres3SignTargetDTO, DTO);

    // 药品申请单ID 对应 med_medication_request.id
    API_DTO_FIELD_REQUIRE(String, medicationRequestId, ZH_WORDS_GETTER("chinapres3.field.medicationRequestId"), true);

    // 处方分组标识，用于区分同一批次多张处方
    // API_DTO_FIELD_REQUIRE(String, prescriptionGroupKey, ZH_WORDS_GETTER("chinapres3.field.prescriptionGroupKey"), true);
};

#include OATPP_CODEGEN_END(DTO)