#ifndef _PHARMADDDTO_H_
#define _PHARMADDDTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PayloadDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class PharmAddDTO : public oatpp::DTO
{
    DTO_INIT(PharmAddDTO, DTO);

    // ±ØÌî×Ö¶Î
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, type, ZH_WORDS_GETTER("pharm.field.type"), true);

    // Ñ¡Ìî×Ö¶Î
    API_DTO_FIELD_DEFAULT(oatpp::Int32, sort, ZH_WORDS_GETTER("pharm.field.sort"));
    API_DTO_FIELD_DEFAULT(oatpp::String, remark, ZH_WORDS_GETTER("pharm.field.remark"));

    // JWTÔØºÉ×¢Èë
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate()
    {
        if (!name || name->empty())
            return "name invalidate.";
        if (!type || type->empty())
            return "type invalidate.";
        return "";
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMADDDTO_H_