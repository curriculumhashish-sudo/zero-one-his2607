#pragma once
#ifndef _CLN_CONDITION_ADD_DTO_H_
#define _CLN_CONDITION_ADD_DTO_H_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 临床病情添加DTO类
 */
class ClnConditionAddDTO : public oatpp::DTO
{
    DTO_INIT(ClnConditionAddDTO, DTO);

    // 租户标识
    API_DTO_FIELD_REQUIRE(String, tenantId, ZH_WORDS_GETTER("clnCondition.field.tenantId"), true);
    // 就诊标识
    API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("clnCondition.field.encounterId"), true);
    // 描述（诊断描述）
    API_DTO_FIELD_REQUIRE(String, description, ZH_WORDS_GETTER("clnCondition.field.description"), true);
    // 是否中医诊断
    API_DTO_FIELD_REQUIRE(Int32, tcmFlag, ZH_WORDS_GETTER("clnCondition.field.tcmFlag"), false);
  

    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate()
    {
        if (!tenantId || tenantId->empty())
            return "tenantId invalidate.";
        if (!encounterId || encounterId->empty())
            return "encounterId invalidate.";
        if (!description || description->empty())
            return "description invalidate.";
        return "";
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CLN_CONDITION_ADD_DTO_H_