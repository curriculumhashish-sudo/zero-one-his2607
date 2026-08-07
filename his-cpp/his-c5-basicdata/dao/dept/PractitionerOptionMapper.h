#ifndef _PRACTITIONER_OPTION_MAPPER_H_
#define _PRACTITIONER_OPTION_MAPPER_H_

#include "Mapper.h"
#include "domain/dto/dept/PractitionerOptionDTO.h"

class PractitionerOptionMapper
    : public Mapper<PractitionerOptionDTO::Wrapper>
{
public:
    PractitionerOptionDTO::Wrapper mapper(ResultSet* resultSet) const override;
};

#endif // !_PRACTITIONER_OPTION_MAPPER_H_
