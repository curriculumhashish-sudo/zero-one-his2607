#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/26 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _PROCEDURE_DAO_
#define _PROCEDURE_DAO_

#include <list>
#include <string>

#include "BaseDAO.h"
#include "domain/do/medicalorderexecution/ProcedureDO.h"

/**
 * DAO for clinical procedure records (table: cln_procedure).
 * Provides query and update operations for procedure execution records.
 */
class ProcedureDAO : public BaseDAO
{
public:
    ProcedureDAO() = default;
    ~ProcedureDAO() = default;

    /**
     * Query procedures by encounter ID with optional status filter and pagination.
     * @param encounterId the encounter (visit) identifier
     * @param statusEnum optional status filter (COMPLETED / STOPPED / CANCEL)
     * @param pageIndex pagination page index (1-based)
     * @param pageSize pagination page size
     * @return list of procedure records
     */
    std::list<PtrClnProcedureDO> queryByEncounter(
        const std::string& encounterId,
        const std::string& statusEnum,
        uint64_t pageIndex,
        uint64_t pageSize);

    /**
     * Count procedures by encounter ID with optional status filter.
     * @param encounterId the encounter (visit) identifier
     * @param statusEnum optional status filter
     * @return total count
     */
    uint64_t countByEncounter(
        const std::string& encounterId,
        const std::string& statusEnum);

    /**
     * Query a single procedure record by primary key.
     * @param procedureId the procedure primary key
     * @return procedure record or nullptr if not found
     */
    PtrClnProcedureDO queryById(const std::string& procedureId);

    /**
     * Insert a new procedure record (status = COMPLETED).
     * @param procedure the procedure DO to insert
     * @return rows affected (1 = success)
     */
    int insertCompleted(const ClnProcedureDO& procedure);

    /**
     * Update procedure status and status reason.
     * @param procedureId the procedure primary key
     * @param statusEnum new status (STOPPED / CANCEL)
     * @param statusReasonText reason for the status change
     * @param operatorId the operator performing the update
     * @return rows affected (1 = success)
     */
    int updateStatus(
        const std::string& procedureId,
        const std::string& statusEnum,
        const std::string& statusReasonText,
        const std::string& operatorId);
};

#endif // !_PROCEDURE_DAO_
