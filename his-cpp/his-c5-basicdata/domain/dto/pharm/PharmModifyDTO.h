#ifndef _PHARMMODIFYDTO_H_
#define _PHARMMODIFYDTO_H_

#include "../../GlobalInclude.h"
// 修正：公共库鉴权类使用标准包含路径
#include "domain/dto/PayloadDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PharmModifyDTO : public oatpp::DTO
{
    DTO_INIT(PharmModifyDTO, DTO);

    // 必填字段
    API_DTO_FIELD_REQUIRE(oatpp::UInt64, id, ZH_WORDS_GETTER("pharm.field.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, type, ZH_WORDS_GETTER("pharm.field.type"), true);

    // 选填字段
    API_DTO_FIELD_DEFAULT(oatpp::Int32, sort, ZH_WORDS_GETTER("pharm.field.sort"), 0);
    API_DTO_FIELD_DEFAULT(oatpp::String, remark, ZH_WORDS_GETTER("pharm.field.remark"), "");
    // 新增：状态字段，和其他选填字段格式对齐
    API_DTO_FIELD_DEFAULT(oatpp::Int32, status, ZH_WORDS_GETTER("pharm.field.status"), 0);

    // JWT载荷注入
    CC_SYNTHESIZE(PayloadDTO, payload, Payload);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMMODIFYDTO_H_