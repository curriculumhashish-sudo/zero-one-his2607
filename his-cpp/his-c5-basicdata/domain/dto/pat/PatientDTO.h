#pragma once
#ifndef _PATIENT_DTO_
#define _PATIENT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Request body for creating a patient.
 * Server-owned fields such as id, tenant, audit data and version are not accepted.
 */
class PatientAddDTO : public oatpp::DTO
{
    DTO_INIT(PatientAddDTO, DTO);

    DTO_FIELD(String, busNo);
    DTO_FIELD(String, name);
    DTO_FIELD(String, nameJson);
    DTO_FIELD(String, genderEnum);
    DTO_FIELD(String, birthDate);
    DTO_FIELD(String, statusEnum) = "ACTIVE";
    DTO_FIELD(Boolean, activeFlag) = true;
    DTO_FIELD(Boolean, tempFlag) = false;
    DTO_FIELD(String, idCard);
    DTO_FIELD(String, maritalStatusEnum);
    DTO_FIELD(String, prfsEnum);
    DTO_FIELD(String, phone);
    DTO_FIELD(String, address);
    DTO_FIELD(String, addressProvince);
    DTO_FIELD(String, addressCity);
    DTO_FIELD(String, addressDistrict);
    DTO_FIELD(String, addressStreet);
    DTO_FIELD(String, addressExtendJson);
    DTO_FIELD(String, nationalityCode);
    DTO_FIELD(String, pyStr);
    DTO_FIELD(String, wbStr);
    DTO_FIELD(Int32, bloodTypeAbo);
    DTO_FIELD(Int32, bloodTypeRh);
    DTO_FIELD(String, workCompany);
    DTO_FIELD(String, nativePlace);
    DTO_FIELD(String, countryCode);
    DTO_FIELD(String, linkName);
    DTO_FIELD(Int32, linkRelationCode);
    DTO_FIELD(String, linkTelcom);
    DTO_FIELD(String, linkJsons);
    DTO_FIELD(String, organizationId);

    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate() const
    {
        if (!name || name->empty()) return "name is required.";
        if (name->size() > 255) return "name must not exceed 255 characters.";
        if (!genderEnum || genderEnum->empty()) return "genderEnum is required.";
        if (genderEnum->size() > 255) return "genderEnum must not exceed 255 characters.";
        if (!statusEnum || statusEnum->empty()) return "statusEnum is required.";
        if (statusEnum->size() > 255) return "statusEnum must not exceed 255 characters.";
        if (activeFlag.get() == nullptr) return "activeFlag is required.";
        if (tempFlag.get() == nullptr) return "tempFlag is required.";
        if (!organizationId || organizationId->empty()) return "organizationId is required.";
        if (organizationId->size() > 64) return "organizationId must not exceed 64 characters.";
        if (busNo && busNo->size() > 255) return "busNo must not exceed 255 characters.";
        if (idCard && idCard->size() > 255) return "idCard must not exceed 255 characters.";
        if (birthDate && !birthDate->empty() && birthDate->size() != 10)
            return "birthDate must use yyyy-MM-dd format.";
        return "";
    }
};

/**
 * Patch request for updating a patient.
 * A null field is left unchanged; an empty string clears a nullable string field.
 */
class PatientUpdateDTO : public oatpp::DTO
{
    DTO_INIT(PatientUpdateDTO, DTO);

    DTO_FIELD(String, id);
    DTO_FIELD(Int32, version);
    DTO_FIELD(String, busNo);
    DTO_FIELD(String, name);
    DTO_FIELD(String, nameJson);
    DTO_FIELD(String, genderEnum);
    DTO_FIELD(String, birthDate);
    DTO_FIELD(String, statusEnum);
    DTO_FIELD(Boolean, activeFlag);
    DTO_FIELD(Boolean, tempFlag);
    DTO_FIELD(String, idCard);
    DTO_FIELD(String, maritalStatusEnum);
    DTO_FIELD(String, prfsEnum);
    DTO_FIELD(String, phone);
    DTO_FIELD(String, address);
    DTO_FIELD(String, addressProvince);
    DTO_FIELD(String, addressCity);
    DTO_FIELD(String, addressDistrict);
    DTO_FIELD(String, addressStreet);
    DTO_FIELD(String, addressExtendJson);
    DTO_FIELD(String, nationalityCode);
    DTO_FIELD(String, pyStr);
    DTO_FIELD(String, wbStr);
    DTO_FIELD(Int32, bloodTypeAbo);
    DTO_FIELD(Int32, bloodTypeRh);
    DTO_FIELD(String, workCompany);
    DTO_FIELD(String, nativePlace);
    DTO_FIELD(String, countryCode);
    DTO_FIELD(String, linkName);
    DTO_FIELD(Int32, linkRelationCode);
    DTO_FIELD(String, linkTelcom);
    DTO_FIELD(String, linkJsons);
    DTO_FIELD(String, deceasedDate);
    DTO_FIELD(String, deceasedReason);
    DTO_FIELD(String, organizationId);
    DTO_FIELD(String, mergedTargetPatientId);
    DTO_FIELD(String, mergedAt);

    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate() const
    {
        if (!id || id->empty()) return "id is required.";
        if (id->size() > 64) return "id must not exceed 64 characters.";
        if (!version) return "version is required.";
        if (version.getValue(-1) < 0) return "version must be greater than or equal to 0.";
        if (name && name->empty()) return "name must not be empty.";
        if (name && name->size() > 255) return "name must not exceed 255 characters.";
        if (genderEnum && genderEnum->empty()) return "genderEnum must not be empty.";
        if (statusEnum && statusEnum->empty()) return "statusEnum must not be empty.";
        if (organizationId && organizationId->empty()) return "organizationId must not be empty.";
        if (organizationId && organizationId->size() > 64)
            return "organizationId must not exceed 64 characters.";
        if (busNo && busNo->empty()) return "busNo must not be empty.";
        if (busNo && busNo->size() > 255) return "busNo must not exceed 255 characters.";
        if (idCard && idCard->size() > 255) return "idCard must not exceed 255 characters.";
        if (birthDate && !birthDate->empty() && birthDate->size() != 10)
            return "birthDate must use yyyy-MM-dd format.";
        return "";
    }
};

/**
 * Patient detail returned to clients. Tenant, deletion and audit fields are intentionally hidden.
 */
class PatientDetailDTO : public oatpp::DTO
{
    DTO_INIT(PatientDetailDTO, DTO);

    DTO_FIELD(String, id);
    DTO_FIELD(Int32, version);
    DTO_FIELD(String, busNo);
    DTO_FIELD(String, name);
    DTO_FIELD(String, nameJson);
    DTO_FIELD(String, genderEnum);
    DTO_FIELD(String, birthDate);
    DTO_FIELD(String, statusEnum);
    DTO_FIELD(Boolean, activeFlag);
    DTO_FIELD(Boolean, tempFlag);
    DTO_FIELD(String, idCard);
    DTO_FIELD(String, maritalStatusEnum);
    DTO_FIELD(String, prfsEnum);
    DTO_FIELD(String, phone);
    DTO_FIELD(String, address);
    DTO_FIELD(String, addressProvince);
    DTO_FIELD(String, addressCity);
    DTO_FIELD(String, addressDistrict);
    DTO_FIELD(String, addressStreet);
    DTO_FIELD(String, addressExtendJson);
    DTO_FIELD(String, nationalityCode);
    DTO_FIELD(String, pyStr);
    DTO_FIELD(String, wbStr);
    DTO_FIELD(Int32, bloodTypeAbo);
    DTO_FIELD(Int32, bloodTypeRh);
    DTO_FIELD(String, workCompany);
    DTO_FIELD(String, nativePlace);
    DTO_FIELD(String, countryCode);
    DTO_FIELD(String, linkName);
    DTO_FIELD(Int32, linkRelationCode);
    DTO_FIELD(String, linkTelcom);
    DTO_FIELD(String, linkJsons);
    DTO_FIELD(String, deceasedDate);
    DTO_FIELD(String, deceasedReason);
    DTO_FIELD(String, organizationId);
    DTO_FIELD(String, mergedTargetPatientId);
    DTO_FIELD(String, mergedAt);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PATIENT_DTO_
