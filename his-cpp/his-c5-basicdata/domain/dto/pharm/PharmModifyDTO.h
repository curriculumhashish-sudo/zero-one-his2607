#ifndef _PHARMMODIFYDTO_H_
#define _PHARMMODIFYDTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PayloadDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class PharmModifyDTO : public oatpp::DTO
{
    DTO_INIT(PharmModifyDTO, DTO);

    // 必填字段
    API_DTO_FIELD_REQUIRE(oatpp::UInt64, id, ZH_WORDS_GETTER("pharm.field.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, type, ZH_WORDS_GETTER("pharm.field.type"), true);

    // 选填字段（禁止手动传第4个默认值参数）
    API_DTO_FIELD_DEFAULT(oatpp::Int32, sort, ZH_WORDS_GETTER("pharm.field.sort"));
    API_DTO_FIELD_DEFAULT(oatpp::String, remark, ZH_WORDS_GETTER("pharm.field.remark"));
    API_DTO_FIELD_DEFAULT(oatpp::Int32, status, ZH_WORDS_GETTER("pharm.field.status"));

    // JWT载荷注入，与 AddDTO 写法完全对齐
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMMODIFYDTO_H_