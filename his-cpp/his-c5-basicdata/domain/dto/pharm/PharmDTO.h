#ifndef _PHARMDTO_H_
#define _PHARMDTO_H_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class PharmDTO : public oatpp::DTO
{
    DTO_INIT(PharmDTO, DTO);

    // ±ØÌî×Ö¶Î
    API_DTO_FIELD_REQUIRE(oatpp::UInt64, id, ZH_WORDS_GETTER("pharm.field.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("pharm.field.name"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, type, ZH_WORDS_GETTER("pharm.field.type"), true);
    API_DTO_FIELD_REQUIRE(oatpp::Int32, status, ZH_WORDS_GETTER("pharm.field.status"), true);

    // Ñ¡Ìî×Ö¶Î
    API_DTO_FIELD_DEFAULT(oatpp::Int32, sort, ZH_WORDS_GETTER("pharm.field.sort"));
    API_DTO_FIELD_DEFAULT(oatpp::String, remark, ZH_WORDS_GETTER("pharm.field.remark"));
    API_DTO_FIELD_DEFAULT(oatpp::String, create_by, ZH_WORDS_GETTER("pharm.field.createBy"));
    API_DTO_FIELD_DEFAULT(oatpp::String, update_by, ZH_WORDS_GETTER("pharm.field.updateBy"));
    API_DTO_FIELD_DEFAULT(oatpp::String, create_time, ZH_WORDS_GETTER("pharm.field.createTime"));
    API_DTO_FIELD_DEFAULT(oatpp::String, update_time, ZH_WORDS_GETTER("pharm.field.updateTime"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PHARMDTO_H_