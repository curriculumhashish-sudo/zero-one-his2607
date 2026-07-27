#ifndef _ORGANIZATIONDETAILDTO_H_
#define _ORGANIZATIONDETAILDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationDetailDTO : public oatpp::DTO
{
    DTO_INIT(OrganizationDetailDTO, DTO);

    API_DTO_FIELD_REQUIRE(oatpp::String, id, ZH_WORDS_GETTER("dept.organizationQuery.fields.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("dept.organizationQuery.fields.name"), true);
    API_DTO_FIELD_DEFAULT(oatpp::String, parentId, ZH_WORDS_GETTER("dept.organizationQuery.fields.parentId"));
    API_DTO_FIELD_DEFAULT(oatpp::String, defDoctorId, ZH_WORDS_GETTER("dept.organizationQuery.fields.defDoctorId"));
    API_DTO_FIELD_DEFAULT(oatpp::String, busNo, ZH_WORDS_GETTER("dept.organizationQuery.fields.busNo"));
    API_DTO_FIELD_DEFAULT(oatpp::String, caty, ZH_WORDS_GETTER("dept.organizationQuery.fields.caty"));
    API_DTO_FIELD_DEFAULT(oatpp::String, typeEnum, ZH_WORDS_GETTER("dept.organizationQuery.fields.typeEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, classEnum, ZH_WORDS_GETTER("dept.organizationQuery.fields.classEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, statusEnum, ZH_WORDS_GETTER("dept.organizationQuery.fields.statusEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, ybNo, ZH_WORDS_GETTER("dept.organizationQuery.fields.ybNo"));
    API_DTO_FIELD_DEFAULT(oatpp::String, ybName, ZH_WORDS_GETTER("dept.organizationQuery.fields.ybName"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medinsId, ZH_WORDS_GETTER("dept.organizationQuery.fields.medinsId"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medinsAdmdvs, ZH_WORDS_GETTER("dept.organizationQuery.fields.medinsAdmdvs"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medinsType, ZH_WORDS_GETTER("dept.organizationQuery.fields.medinsType"));
    API_DTO_FIELD_DEFAULT(oatpp::String, medinsLv, ZH_WORDS_GETTER("dept.organizationQuery.fields.medinsLv"));
    API_DTO_FIELD_DEFAULT(oatpp::String, pyStr, ZH_WORDS_GETTER("dept.organizationQuery.fields.pyStr"));
    API_DTO_FIELD_DEFAULT(oatpp::String, wbStr, ZH_WORDS_GETTER("dept.organizationQuery.fields.wbStr"));
    API_DTO_FIELD_DEFAULT(oatpp::Int32, displayOrder, ZH_WORDS_GETTER("dept.organizationQuery.fields.displayOrder"));
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONDETAILDTO_H_
