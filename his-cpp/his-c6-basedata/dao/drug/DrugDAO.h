#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/23

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
#ifndef _DRUG_DAO_H_
#define _DRUG_DAO_H_

#include "BaseDAO.h"
#include "../../domain/do/drug/MedMedicationDefinitionDO.h"
#include "../../domain/do/adm/AdmChargeItemDefinitionDO.h"
#include "DrugMapper.h"
#include "domain/query/Drug/DrugQuery.h"
/**
 * Persistence operations for the drug aggregate and its base prices.
 */
class DrugDAO : public BaseDAO
{
  private:
    static std::string fullDrugColumns();
    static std::string placeholders(size_t count);
    static void appendIds(SqlParams &params, const std::list<std::string> &ids);

  public:
    void releaseSqlSession() noexcept;

    bool hasTenantAccess(const std::string &userId, const std::string &username,
                         const std::string &tenantId);
    PtrMedMedicationDefinitionDO selectById(const std::string &id, bool forUpdate = false);
    std::list<PtrMedMedicationDefinitionDO> selectStatesForUpdate(const std::list<std::string> &ids);
    int updateEditableByVersion(const MedMedicationDefinitionDO &data, const std::string &tenantId,
                                int expectedVersion);

    int softDeleteBasePrices(const std::string &drugId, const std::string &tenantId, const std::string &operatorId,
                             const std::string &operationTime);
    int insertBasePrice(const AdmChargeItemDefinitionDO &price);
    std::list<PtrAdmChargeItemDefinitionDO> selectBasePrices(const std::string &drugId,
                                                             const std::string &tenantId);

    PtrDrugReferenceCheck selectReferenceCheck(const std::list<std::string> &ids);
    PtrDrugConditionCheck selectEnableReadiness(const std::list<std::string> &ids, const std::string &tenantId);
    PtrDrugConditionCheck selectActivePricePresence(const std::list<std::string> &ids,
                                                    const std::string &tenantId);
    int softDeleteAllPrices(const std::list<std::string> &ids, const std::string &tenantId,
                            const std::string &operatorId, const std::string &operationTime);
    int softDeleteDrugs(const std::list<std::string> &ids, const std::string &tenantId,
                        const std::string &operatorId,
                        const std::string &operationTime);
    int updateActiveStatus(const std::list<std::string> &ids, const std::string &tenantId,
                           const std::string &expectedStatus,
                           const std::string &targetStatus, bool activeFlag, const std::string &operatorId,
                           const std::string &operationTime);
    uint64_t count(const DrugQuery::Wrapper& query);
    // 分页查询数据
    std::list<MedMedicationDefinitionDO> selectWithPage(const DrugQuery::Wrapper& query);
    // 通过姓名查询数据
    std::list<MedMedicationDefinitionDO> selectByName(const string& name);
    // 通过ID查询数据
    MedMedicationDefinitionDO selectById(std::string id);
    // 新增的数据插入 (供 saveData 调用)
    int insert(const MedMedicationDefinitionDO& entity);





};

#endif // !_DRUG_DAO_H_
