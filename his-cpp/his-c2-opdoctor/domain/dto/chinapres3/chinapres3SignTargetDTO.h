#pragma once
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 签收目标DTO
 *
 * 定义单个签收目标的标识信息，用于指定要对哪些处方申请记录执行签收操作。
 * 每次签收操作需同时提供 medicationRequestId（处方药品申请ID）和
 * prescriptionGroupKey（处方分组键），以确保精确匹配目标记录。
 */
class chinapres3SignTargetDTO : public oatpp::DTO
{
    DTO_INIT(chinapres3SignTargetDTO, DTO);

    // 处方药品申请ID，对应 med_medication_request.id，唯一标识一条处方药品申请记录
    API_DTO_FIELD_REQUIRE(String, medicationRequestId, ZH_WORDS_GETTER("chinapres3.field.medicationRequestId"), true);

    // 处方分组键，用于标识同一张处方的分组关系，确保签发时同一处方的多条记录一致处理
    API_DTO_FIELD_REQUIRE(String, prescriptionGroupKey, ZH_WORDS_GETTER("chinapres3.field.prescriptionGroupKey"), true);
};

#include OATPP_CODEGEN_END(DTO)
