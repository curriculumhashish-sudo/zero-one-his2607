#ifndef _ORGANIZATIONDETAILDTO_H_
#define _ORGANIZATIONDETAILDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationDetailDTO : public oatpp::DTO
{
    DTO_INIT(OrganizationDetailDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, "Organization id", true);
    API_DTO_FIELD_REQUIRE(String, name, "Organization name", true);
    API_DTO_FIELD_DEFAULT(String, type_enum, "Organization type");
    API_DTO_FIELD_DEFAULT(String, class_enum, "Organization category");
    API_DTO_FIELD_DEFAULT(String, status_enum, "Organization status");
    API_DTO_FIELD_DEFAULT(String, yb_no, "Medical insurance code");
    API_DTO_FIELD_DEFAULT(String, yb_name, "Medical insurance name");
    API_DTO_FIELD_DEFAULT(String, medins_id, "Medical institution id");
    API_DTO_FIELD_DEFAULT(String, medins_admdvs, "Medical insurance area code");
    API_DTO_FIELD_DEFAULT(String, medins_type, "Medical institution type");
    API_DTO_FIELD_DEFAULT(String, medins_lv, "Medical institution level");
    API_DTO_FIELD_DEFAULT(String, py_str, "Pinyin code");
    API_DTO_FIELD_DEFAULT(String, wb_str, "Wubi code");
    API_DTO_FIELD_DEFAULT(Int32, display_order, "Display order");
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONDETAILDTO_H_