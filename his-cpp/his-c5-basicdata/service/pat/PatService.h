#pragma once
#ifndef _PAT_SERVICE_H_
#define _PAT_SERVICE_H_

#include "../../domain/query/pat/PatSelectPageQuery.h"
#include "../../domain/query/pat/PatPageQuery.h"
#include "../../domain/dto/pat/PatSelectDTO.h"
#include "../../domain/dto/pat/PatDTO.h"
#include "domain/dto/PageDTO.h"
#include "domain/dto/PayloadDTO.h"
#include <list>
#include <string>

/**
 * Patient business service.
 */
class PatService
{
public:
    // ============================================================
    // Query methods (c5)
    // ============================================================

    /** Paginated query for select-patient popup. */
    PageDTO<PatSelectDTO::Wrapper>::Wrapper querySelectPatPage(const PatSelectPageQuery::Wrapper& query);

    /** Paginated query for patient management list (GET /pat/page). */
    PageDTO<PatDTO::Wrapper>::Wrapper queryPatPage(const PatPageQuery::Wrapper& query);

    // ============================================================
    // Soft-delete patient (c5)
    // Set is_deleted=1, no physical deletion
    // Params: id=patient ID, payload=JWT auth info (for audit)
    // Returns: true=success, false=record not found or already deleted
    // ============================================================
    bool removePatient(const std::string& id, const PayloadDTO& payload);

    // ============================================================
    // Batch delete methods (c5-pat-delete-pb)
    // ============================================================

    /** Validate delete parameters, returns error message or empty string. */
    std::string validateRemove(const oatpp::List<oatpp::String>& ids);

    /** Batch delete patients by ID list, returns true if all deleted. */
    bool removeData(const oatpp::List<oatpp::String>& ids);

private:
    /** Convert gender_enum (MALE/FEMALE) to display text. */
    static std::string toGenderLabel(const std::string& genderEnum);
    /** Convert gender_enum (MALE/FEMALE) to numeric code (MALE->1, FEMALE->2, unknown->0). */
    static int toGenderCode(const std::string& genderEnum);
    /** Calculate age from birth_date (yyyy-MM-dd). Empty/invalid returns 0. */
    static int calcAge(const std::string& birthDate);
};

#endif
