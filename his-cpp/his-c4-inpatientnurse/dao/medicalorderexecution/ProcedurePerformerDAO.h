#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: fanpai
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
#ifndef _PROCEDURE_PERFORMER_DAO_
#define _PROCEDURE_PERFORMER_DAO_

#include <list>
#include <string>

#include "BaseDAO.h"
#include "domain/do/medicalorderexecution/ProcedurePerformerDO.h"

/**
 * DAO for procedure performer records (table: cln_procedure_performer).
 * Manages the healthcare workers assigned to a clinical procedure.
 */
class ProcedurePerformerDAO : public BaseDAO
{
public:
    ProcedurePerformerDAO() = default;
    ~ProcedurePerformerDAO() = default;

    /**
     * Query all performers assigned to a procedure.
     * @param procedureId the procedure primary key
     * @return list of performer records
     */
    std::list<PtrProcedurePerformerDO> queryByProcedureId(const std::string& procedureId);

    /**
     * Insert a new procedure performer assignment.
     * @param performer the performer DO to insert
     * @return rows affected (1 = success)
     */
    int insert(const ProcedurePerformerDO& performer);

    /**
     * Insert multiple performers in batch for a single procedure.
     * @param performers list of performer DOs to insert
     * @return rows affected
     */
    int insertBatch(const std::list<ProcedurePerformerDO>& performers);

    /**
     * Remove all performers for a procedure (logical delete by procedure_id).
     * @param procedureId the procedure primary key
     * @param operatorId the operator performing the deletion
     * @return rows affected
     */
    int removeByProcedureId(const std::string& procedureId, const std::string& operatorId);
};

#endif // !_PROCEDURE_PERFORMER_DAO_
