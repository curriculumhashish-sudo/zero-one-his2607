#pragma once
#ifndef _BED_DAO_
#define _BED_DAO_
#include "BaseDAO.h"
#include "../../domain/do/admissiontransfer/adm_locationDO.h"
#include "../../domain/do/admissiontransfer/bkg_bed_reservation_extDO.h"
#include "../../domain/query/admissiontransfer/BedQuery.h"

// Bed DAO: query free beds + reserve(assign) a bed
class BedDAO : public BaseDAO
{
private:
    // Build the shared WHERE clause (free-bed condition)
    inline std::string queryConditionBuilder(const BedQuery::Wrapper& query, SqlParams& params);
public:
    // Count free beds
    uint64_t countFree(const BedQuery::Wrapper& query);
    // Query free beds with pagination
    std::list<adm_locationDO> selectFreeWithPage(const BedQuery::Wrapper& query);
    // Insert a reservation record (assign). Returns affected rows.
    int reserve(const bkg_bed_reservation_extDO& data);
};

#endif // !_BED_DAO_
