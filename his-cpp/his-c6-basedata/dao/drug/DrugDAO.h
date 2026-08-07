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
#include "domain/query/Drug/DrugSelectQuery.h"

/**
 * Persistence operations for the drug aggregate and its base prices.
 */
class DrugDAO : public BaseDAO
{
  private:
    static std::string fullDrugColumns();
    static std::string placeholders(size_t count);
    static void appendIds(SqlParams &params, const std::list<std::string> &ids);
    std::string queryConditionBuilder(const DrugQuery::Wrapper &query, SqlParams &params);
    std::string querySelectConditionBuilder(const DrugSelectQuery::Wrapper& query, SqlParams& params);

  public:
    // 提前释放当前 DAO 持有的数据库会话。
    void releaseSqlSession() noexcept;

    // 校验当前用户是否可以访问指定租户的数据。
    bool hasTenantAccess(const std::string &userId, const std::string &username,
                         const std::string &tenantId);
    // 解析当前登录用户唯一有效的租户和医院机构。
    std::string selectCurrentTenantId(const std::string &userId);
    std::string selectCurrentHospitalOrgId(const std::string &userId,
                                           const std::string &tenantId);
    // 按主键查询药品，可选择加行锁。
    PtrMedMedicationDefinitionDO selectById(const std::string &id, bool forUpdate = false);
    std::list<PtrMedMedicationDefinitionDO> selectStatesForUpdate(const std::list<std::string> &ids);
    // 按版本更新请求中显式传入的药品字段。
    int updateEditableByVersion(const MedMedicationDefinitionDO &data, const std::string &tenantId,
                                int expectedVersion);
    // 更新指定价格类型的基础价格。
    int updateBasePriceValue(const std::string &drugId, const std::string &tenantId,
                             const std::string &priceType, double priceValue,
                             const std::string &operatorId, const std::string &operationTime);
    int updateBasePriceFinanceType(const std::string &drugId, const std::string &tenantId,
                                   const std::string &financeTypeCode,
                                   const std::string &operatorId, const std::string &operationTime);
    int updateBasePriceInsuranceType(const std::string &drugId, const std::string &tenantId,
                                     const std::string &insuranceType,
                                     const std::string &operatorId, const std::string &operationTime);
    int updateBasePriceChargeName(const std::string &drugId, const std::string &tenantId,
                                  const std::string &chargeName,
                                  const std::string &operatorId, const std::string &operationTime);

    int softDeleteBasePrices(const std::string &drugId, const std::string &tenantId, const std::string &operatorId,
                             const std::string &operationTime);
    int insertBasePrice(const AdmChargeItemDefinitionDO &price);
    std::list<PtrAdmChargeItemDefinitionDO> selectBasePrices(const std::string &drugId,
                                                             const std::string &tenantId);
    PtrDrugModifyTextResult selectModifyResultTexts(const std::string &drugId,
                                                    const std::string &tenantId,
                                                    const std::string &typeCode);

    // 检查药品是否仍被医嘱、发药、过敏、模板或库存引用。
    PtrDrugReferenceCheck selectReferenceCheck(const std::list<std::string> &ids);
    // 检查停用药品是否满足重新启用条件。
    PtrDrugConditionCheck selectEnableReadiness(const std::list<std::string> &ids, const std::string &tenantId);
    // 检查药品是否还存在未删除的价格记录。
    PtrDrugConditionCheck selectActivePricePresence(const std::list<std::string> &ids,
                                                     const std::string &tenantId);
    // 软删除指定药品关联的全部价格记录。
    int softDeleteAllPrices(const std::list<std::string> &ids, const std::string &tenantId,
                            const std::string &operatorId, const std::string &operationTime);
    int softDeleteDrugs(const std::list<std::string> &ids, const std::string &tenantId,
                        const std::string &operatorId,
                        const std::string &operationTime);
    // 按版本软删除单个停用药品。
    int softDeleteDrugByVersion(const std::string &id, const std::string &tenantId,
                                int expectedVersion, const std::string &operatorId,
                                const std::string &operationTime);
    int updateActiveStatus(const std::list<std::string> &ids, const std::string &tenantId,
                           const std::string &expectedStatus,
                           const std::string &targetStatus, bool activeFlag, const std::string &operatorId,
                           const std::string &operationTime);
    // 按版本切换单个药品的启停状态。
    int updateActiveStatusByVersion(const std::string &id, const std::string &tenantId,
                                    int expectedVersion, const std::string &expectedStatus,
                                    const std::string &targetStatus, bool activeFlag,
                                    const std::string &operatorId, const std::string &operationTime);
    uint64_t count(const DrugQuery::Wrapper &query);
    uint64_t count2(const DrugSelectQuery::Wrapper &query);
    std::list<PtrMedMedicationDefinitionDO> selectWithPage(const DrugQuery::Wrapper &query);
    std::list<PtrMedMedicationDefinitionDO> SElectWithPage(const DrugSelectQuery::Wrapper& query);
    int insert(const MedMedicationDefinitionDO &entity);
};

#endif // !_DRUG_DAO_H_
