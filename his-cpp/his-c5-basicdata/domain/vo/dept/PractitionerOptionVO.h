#ifndef _PRACTITIONER_OPTION_VO_H_
#define _PRACTITIONER_OPTION_VO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/dept/PractitionerOptionDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Success envelope for the doctor option endpoint.
 * A successful empty result is represented by data: [].
 */
class PractitionerOptionJsonVO
    : public JsonVO<oatpp::List<PractitionerOptionDTO::Wrapper>>
{
    DTO_INIT(
        PractitionerOptionJsonVO,
        JsonVO<oatpp::List<PractitionerOptionDTO::Wrapper>>
    );

public:
    PractitionerOptionJsonVO()
    {
        data = {};
    }
};

/**
 * Failure envelope for the doctor option endpoint.
 * The data field is null for failures.
 */
class PractitionerOptionErrorVO : public JsonVO<oatpp::String>
{
    DTO_INIT(PractitionerOptionErrorVO, JsonVO<oatpp::String>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRACTITIONER_OPTION_VO_H_
