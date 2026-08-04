#pragma once
#ifndef _PATIENT_QUERY_H_
#define _PATIENT_QUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/** Query parameters for a tenant-scoped patient detail lookup. */
class PatientDetailQuery : public oatpp::DTO
{
    DTO_INIT(PatientDetailQuery, DTO);

    DTO_FIELD(String, id);
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate() const
    {
        if (!id || id->empty()) return "id is required.";
        if (id->size() > 64) return "id must not exceed 64 characters.";
        return "";
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_QUERY_H_
