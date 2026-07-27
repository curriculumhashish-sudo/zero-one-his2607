#ifndef _ORGANIZATIONDETAILDTO_H_
#define _ORGANIZATIONDETAILDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationDetailDTO : public oatpp::DTO
{
    DTO_INIT(OrganizationDetailDTO, DTO);

    API_DTO_FIELD_REQUIRE(oatpp::String, id, ZH_WORDS_GETTER("organization.fields.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("organization.fields.name"), true);
    API_DTO_FIELD_DEFAULT(oatpp::String, type_enum, ZH_WORDS_GETTER("organization.fields.typeEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, class_enum, ZH_WORDS_GETTER("organization.fields.classEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, status_enum, ZH_WORDS_GETTER("organization.fields.statusEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, yb_no, ZH_WORDS_GETTER("organization.fields.ybNo"));
    API_DTO_FIELD_DEFAULT(oatpp::String, yb_name, ZH_WORDS_GETTER("organization.fields.ybName"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medins_id, ZH_WORDS_GETTER("organization.fields.medinsId"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medins_admdvs, ZH_WORDS_GETTER("organization.fields.medinsAdmdvs"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medins_type, ZH_WORDS_GETTER("organization.fields.medinsType"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medins_lv, ZH_WORDS_GETTER("organization.fields.medinsLv"));
    API_DTO_FIELD_DEFAULT(oatpp::String, py_str, ZH_WORDS_GETTER("organization.fields.pyStr"));
    API_DTO_FIELD_DEFAULT(oatpp::String, wb_str, ZH_WORDS_GETTER("organization.fields.wbStr"));
    API_DTO_FIELD_DEFAULT(oatpp::Int32, display_order, ZH_WORDS_GETTER("organization.fields.displayOrder"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONDETAILDTO_H_
