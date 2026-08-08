#ifndef _PRACTITIONER_OPTION_DTO_H_
#define _PRACTITIONER_OPTION_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * A doctor option used by the department owner selector.
 * The database status_enum value is exposed as status without translation.
 */
class PractitionerOptionDTO : public oatpp::DTO
{
    DTO_INIT(PractitionerOptionDTO, DTO);

    API_DTO_FIELD_REQUIRE(
        oatpp::String,
        id,
        ZH_WORDS_GETTER("dept.practitionerOption.fields.id"),
        true
    );
    API_DTO_FIELD_REQUIRE(
        oatpp::String,
        name,
        ZH_WORDS_GETTER("dept.practitionerOption.fields.name"),
        true
    );
    API_DTO_FIELD_REQUIRE(
        oatpp::String,
        status,
        ZH_WORDS_GETTER("dept.practitionerOption.fields.status"),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRACTITIONER_OPTION_DTO_H_
