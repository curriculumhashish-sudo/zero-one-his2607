#pragma once
#ifndef _BED_SERVICE_
#define _BED_SERVICE_
#include "domain/vo/admissiontransfer/BedVo.h"
#include "domain/query/admissiontransfer/BedQuery.h"
#include "domain/dto/admissiontransfer/BedAssignDTO.h"

// Bed service: query free beds + assign a bed
class BedService
{
public:
    // Query free beds with pagination
    BedPageDTO::Wrapper listFree(const BedQuery::Wrapper& query);
    // Assign(reserve) a bed. Returns the new record id, or "" on failure/duplicate.
    std::string assignBed(const BedAssignAddDTO::Wrapper& dto, const PayloadDTO& payload);
};

#endif // !_BED_SERVICE_
