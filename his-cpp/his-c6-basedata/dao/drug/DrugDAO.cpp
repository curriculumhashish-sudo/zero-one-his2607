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
#include "stdafx.h"
#include "DrugDAO.h"
#include "DrugMapper.h"
#include <sstream>
std::string DrugDAO::queryConditionBuilder(const DrugQuery::Wrapper& query, SqlParams& params)
{
    std::stringstream sqlCondition;

    // 基础条件：只查询未删除的数据 (is_deleted = 0)
    sqlCondition << " WHERE `is_deleted` = 0 ";

    // 条件1：关键词模糊查询 
    if (query->keywords && !query->keywords->empty()) {
        sqlCondition << " AND (`name` LIKE CONCAT('%',?,'%') OR `py_str` LIKE CONCAT('%',?,'%')) ";
        SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
        SQLPARAMS_PUSH(params, "s", std::string, query->keywords.getValue(""));
    }

    // 条件2：器材分类精准查询
    if (query->kind && !query->kind->empty()) {
        sqlCondition << " AND `category_code` = ? ";
        SQLPARAMS_PUSH(params, "s", std::string, query->kind.getValue(""));
    }

    return sqlCondition.str();

}

void DrugDAO::releaseSqlSession() noexcept
{
    delete sqlSession;
    sqlSession = nullptr;
    setAutoRelease(false);
}

std::string DrugDAO::fullDrugColumns()
{
    return "id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,deleted_at,deleted_by,version,"
           "bus_no,name,domain_enum,med_version,name_en,py_str,wb_str,category_code,merchandise_name,"
           "merchandise_py_str,merchandise_wb_str,purchase_unit_code,split_unit_code,comprised_text,ingredient,"
           "dose_from,approval_number,yb_match_flag,yb_no,pharmacology_category_code,skin_test_flag,inject_flag,"
           "manufacturer_id,manufacturer_text,supply_id,restricted_flag,restricted_scope,active_flag,children_flag,"
           "characteristic,item_min_quantity,item_max_quantity,def_encounter_unit_code,def_inhospital_unit_code,"
           "national_drug_code,part_attribute_enum,antibiotic_code,restricted_enum,self_flag,antibiotic_flag,"
           "basic_flag,tho_part_attribute_enum,chrgitm_lv,rx_flag,dosage_instruction,drug69_code,total_volume,"
           "dose_form_code,dose_unit_code,package_unit_code,stock_unit_code,method_code,rate_code,dose,max_unit,"
           "usage_limit,location_id,status_enum,org_id,ingredient_item,medicationdefinition,ddd_code,ddd_unit_code,"
           "min_rate_code,max_rate_code,dose_unit_ratio,purchase_unit_ratio,stock_unit_ratio,package_unit_ratio";
}

std::string DrugDAO::placeholders(size_t count)
{
    std::ostringstream result;
    for (size_t index = 0; index < count; ++index)
    {
        if (index != 0)
        {
            result << ",";
        }
        result << "?";
    }
    return result.str();
}

void DrugDAO::appendIds(SqlParams &params, const std::list<std::string> &ids)
{
    for (const auto &id : ids)
    {
        SQLPARAMS_PUSH(params, "s", std::string, id);
    }
}

bool DrugDAO::hasTenantAccess(const std::string &userId, const std::string &username,
                              const std::string &tenantId)
{
    if (tenantId.empty() || (userId.empty() && username.empty()))
    {
        return false;
    }

    std::string sql =
        "SELECT COUNT(ut.id) FROM whale_user_tenants ut "
        "INNER JOIN whale_users u ON u.id=ut.user_id AND u.is_deleted=0 AND u.is_active=1 "
        "AND u.is_locked_out=0 "
        "WHERE ut.is_deleted=0 AND ut.tenant_id=?";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    if (!userId.empty())
    {
        sql += " AND ut.user_id=?";
        SQLPARAMS_PUSH(params, "s", std::string, userId);
    }
    else
    {
        sql += " AND u.user_name=?";
        SQLPARAMS_PUSH(params, "s", std::string, username);
    }
    return sqlSession->executeQueryNumerical(sql, params) > 0;
}

PtrMedMedicationDefinitionDO DrugDAO::selectById(const std::string &id, bool forUpdate)
{
    std::string sql = "SELECT " + fullDrugColumns() +
                      " FROM med_medication_definition WHERE id=? AND is_deleted=0";
    if (forUpdate)
    {
        sql += " FOR UPDATE";
    }
    return sqlSession->executeQueryOne<PtrMedMedicationDefinitionDO>(sql, PtrDrugMapper(), "%s", id);
}

std::list<PtrMedMedicationDefinitionDO> DrugDAO::selectStatesForUpdate(const std::list<std::string> &ids)
{
    if (ids.empty())
    {
        return {};
    }

    SqlParams params;
    appendIds(params, ids);
    const std::string sql =
        "SELECT id,tenant_id,is_deleted,version,status_enum,active_flag "
        "FROM med_medication_definition WHERE id IN (" +
        placeholders(ids.size()) + ") AND is_deleted=0 FOR UPDATE";
    return sqlSession->executeQuery<PtrMedMedicationDefinitionDO>(sql, PtrDrugStateMapper(), params);
}

int DrugDAO::updateEditableByVersion(const MedMedicationDefinitionDO &data, const std::string &tenantId,
                                     int expectedVersion)
{
    if (!data.getPrimaryField()->get())
    {
        return 0;
    }

    SqlParams params;
    std::ostringstream sql;
    sql << "UPDATE med_medication_definition SET ";
    bool first = true;
    for (auto *field : data.getFields())
    {
        if (extractFieldValue(field, &params))
        {
            if (!first)
            {
                sql << ",";
            }
            first = false;
            sql << field->getColumn() << "=?";
        }
    }
    if (first)
    {
        return 0;
    }
    sql << ",version=version+1 WHERE id=? AND tenant_id=? AND version=? AND is_deleted=0 "
           "AND status_enum<>'DISCARDED'";
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);
    return sqlSession->executeUpdate(sql.str(), params);
}

int DrugDAO::softDeleteBasePrices(const std::string &drugId, const std::string &tenantId,
                                  const std::string &operatorId,
                                  const std::string &operationTime)
{
    const std::string sql =
        "UPDATE adm_charge_item_definition SET is_deleted=1,deleted_at=?,deleted_by=?,updated_at=?,updated_by=?,"
        "version=version+1 WHERE instance_table='med_medication_definition' AND instance_id=? AND tenant_id=? "
        "AND is_deleted=0 "
        "AND lot_no IS NULL AND price_type_code IN ('PURCHASE','SALE','LIMIT')";
    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, drugId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeUpdate(sql, params);
}

int DrugDAO::insertBasePrice(const AdmChargeItemDefinitionDO &price)
{
    return insert();
}

std::list<PtrAdmChargeItemDefinitionDO> DrugDAO::selectBasePrices(const std::string &drugId,
                                                                 const std::string &tenantId)
{
    const std::string sql =
        "SELECT price_type_code,price_value,type_code,yb_type FROM adm_charge_item_definition "
        "WHERE instance_table='med_medication_definition' AND instance_id=? AND tenant_id=? AND is_deleted=0 "
        "AND lot_no IS NULL AND price_type_code IN ('PURCHASE','SALE','LIMIT')";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, drugId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeQuery<PtrAdmChargeItemDefinitionDO>(sql, PtrDrugPriceMapper(), params);
}

PtrDrugReferenceCheck DrugDAO::selectReferenceCheck(const std::list<std::string> &ids)
{
    if (ids.empty())
    {
        return nullptr;
    }

    const std::string idPlaceholders = placeholders(ids.size());
    const std::string sql =
        "SELECT CASE WHEN "
        "EXISTS(SELECT id FROM med_medication_request WHERE medication_id IN (" +
        idPlaceholders +
        ") AND is_deleted=0 LIMIT 1) OR "
        "EXISTS(SELECT id FROM med_medication_dispense WHERE medication_id IN (" +
        idPlaceholders +
        ") AND is_deleted=0 LIMIT 1) OR "
        "EXISTS(SELECT id FROM cln_allergy_intolerance WHERE medication_id IN (" +
        idPlaceholders +
        ") AND is_deleted=0 LIMIT 1) OR "
        "EXISTS(SELECT id FROM cus_order_template_line WHERE medication_definition_id IN (" +
        idPlaceholders +
        ") AND is_deleted=0 LIMIT 1) OR "
        "EXISTS(SELECT id FROM wkf_inventory_item WHERE item_table='med_medication_definition' "
        "AND item_id IN (" +
        idPlaceholders +
        ") AND is_deleted=0 AND inventory_status_enum='ACTIVE' AND quantity<>0 LIMIT 1) "
        "THEN 1 ELSE 0 END";

    SqlParams params;
    appendIds(params, ids);
    appendIds(params, ids);
    appendIds(params, ids);
    appendIds(params, ids);
    appendIds(params, ids);
    return sqlSession->executeQueryOne<PtrDrugReferenceCheck>(sql, PtrDrugReferenceCheckMapper(), params);
}

PtrDrugConditionCheck DrugDAO::selectEnableReadiness(const std::list<std::string> &ids,
                                                     const std::string &tenantId)
{
    if (ids.empty())
    {
        return nullptr;
    }

    const std::string sql =
        "SELECT CASE WHEN COUNT(*)=? AND SUM(CASE WHEN "
        "TRIM(COALESCE(d.name,''))<>'' AND TRIM(COALESCE(d.category_code,''))<>'' "
        "AND TRIM(COALESCE(d.merchandise_name,''))<>'' "
        "AND TRIM(COALESCE(d.purchase_unit_code,''))<>'' AND TRIM(COALESCE(d.split_unit_code,''))<>'' "
        "AND TRIM(COALESCE(d.pharmacology_category_code,''))<>'' "
        "AND TRIM(COALESCE(d.manufacturer_text,''))<>'' AND TRIM(COALESCE(d.supply_id,''))<>'' "
        "AND TRIM(COALESCE(d.part_attribute_enum,''))<>'' "
        "AND TRIM(COALESCE(d.tho_part_attribute_enum,''))<>'' "
        "AND TRIM(COALESCE(d.total_volume,''))<>'' "
        "AND TRIM(COALESCE(d.dose_form_code,''))<>'' AND TRIM(COALESCE(d.dose_unit_code,''))<>'' "
        "AND TRIM(COALESCE(d.package_unit_code,''))<>'' AND TRIM(COALESCE(d.stock_unit_code,''))<>'' "
        "AND d.dose_unit_ratio>0 AND d.purchase_unit_ratio>0 AND d.stock_unit_ratio>0 "
        "AND d.package_unit_ratio>0 "
        "AND (d.dose_unit_code<>d.split_unit_code OR d.dose_unit_ratio=1) "
        "AND (d.purchase_unit_code<>d.split_unit_code OR d.purchase_unit_ratio=1) "
        "AND (d.stock_unit_code<>d.split_unit_code OR d.stock_unit_ratio=1) "
        "AND (d.package_unit_code<>d.split_unit_code OR d.package_unit_ratio=1) "
        "AND (d.dose_unit_code<>d.purchase_unit_code OR d.dose_unit_ratio=d.purchase_unit_ratio) "
        "AND (d.dose_unit_code<>d.stock_unit_code OR d.dose_unit_ratio=d.stock_unit_ratio) "
        "AND (d.dose_unit_code<>d.package_unit_code OR d.dose_unit_ratio=d.package_unit_ratio) "
        "AND (d.purchase_unit_code<>d.stock_unit_code OR d.purchase_unit_ratio=d.stock_unit_ratio) "
        "AND (d.purchase_unit_code<>d.package_unit_code OR d.purchase_unit_ratio=d.package_unit_ratio) "
        "AND (d.stock_unit_code<>d.package_unit_code OR d.stock_unit_ratio=d.package_unit_ratio) "
        "AND EXISTS(SELECT p.id FROM adm_charge_item_definition p "
        "WHERE p.instance_table='med_medication_definition' AND p.instance_id=d.id "
        "AND p.tenant_id=d.tenant_id AND p.is_deleted=0 AND p.status_enum='ACTIVE' AND p.lot_no IS NULL "
        "AND p.price_type_code='SALE' AND p.price_value>=0 AND TRIM(COALESCE(p.type_code,''))<>'' LIMIT 1) "
        "THEN 1 ELSE 0 END)=? THEN 1 ELSE 0 END "
        "FROM med_medication_definition d WHERE d.id IN (" +
        placeholders(ids.size()) +
        ") AND d.tenant_id=? AND d.is_deleted=0 AND d.status_enum='INACTIVE'";

    SqlParams params;
    SQLPARAMS_PUSH(params, "i", int, static_cast<int>(ids.size()));
    SQLPARAMS_PUSH(params, "i", int, static_cast<int>(ids.size()));
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeQueryOne<PtrDrugConditionCheck>(sql, PtrDrugConditionCheckMapper(), params);
}

PtrDrugConditionCheck DrugDAO::selectActivePricePresence(const std::list<std::string> &ids,
                                                         const std::string &tenantId)
{
    if (ids.empty())
    {
        return nullptr;
    }

    const std::string sql =
        "SELECT CASE WHEN EXISTS(SELECT id FROM adm_charge_item_definition "
        "WHERE instance_table='med_medication_definition' AND instance_id IN (" +
        placeholders(ids.size()) +
        ") AND tenant_id=? AND is_deleted=0 LIMIT 1) THEN 1 ELSE 0 END";
    SqlParams params;
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeQueryOne<PtrDrugConditionCheck>(sql, PtrDrugConditionCheckMapper(), params);
}

int DrugDAO::softDeleteAllPrices(const std::list<std::string> &ids, const std::string &tenantId,
                                 const std::string &operatorId, const std::string &operationTime)
{
    if (ids.empty())
    {
        return 0;
    }

    const std::string sql =
        "UPDATE adm_charge_item_definition SET is_deleted=1,status_enum='INACTIVE',deleted_at=?,deleted_by=?,"
        "updated_at=?,updated_by=?,version=version+1 WHERE instance_table='med_medication_definition' "
        "AND instance_id IN (" +
        placeholders(ids.size()) + ") AND tenant_id=? AND is_deleted=0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeUpdate(sql, params);
}

int DrugDAO::softDeleteDrugs(const std::list<std::string> &ids, const std::string &tenantId,
                             const std::string &operatorId,
                             const std::string &operationTime)
{
    if (ids.empty())
    {
        return 0;
    }

    const std::string sql =
        "UPDATE med_medication_definition SET is_deleted=1,active_flag=0,status_enum='DISCARDED',"
        "deleted_at=?,deleted_by=?,updated_at=?,updated_by=?,version=version+1 "
        "WHERE id IN (" +
        placeholders(ids.size()) + ") AND tenant_id=? AND is_deleted=0 AND status_enum='INACTIVE'";
    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeUpdate(sql, params);
}

int DrugDAO::updateActiveStatus(const std::list<std::string> &ids, const std::string &tenantId,
                                const std::string &expectedStatus,
                                const std::string &targetStatus, bool activeFlag, const std::string &operatorId,
                                const std::string &operationTime)
{
    if (ids.empty())
    {
        return 0;
    }

    const std::string sql =
        "UPDATE med_medication_definition SET active_flag=?,status_enum=?,updated_at=?,updated_by=?,"
        "version=version+1 WHERE id IN (" +
        placeholders(ids.size()) + ") AND tenant_id=? AND is_deleted=0 AND status_enum=?";
    SqlParams params;
    SQLPARAMS_PUSH(params, "b", bool, activeFlag);
    SQLPARAMS_PUSH(params, "s", std::string, targetStatus);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, expectedStatus);
    return sqlSession->executeUpdate(sql, params);
}
uint64_t count(const DrugQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM wkf_inventory_item ";

    // 拼接条件
    sql += queryConditionBuilder(query, params);

    // 执行查询并返回数字
    return sqlSession->executeQueryNumerical(sql, params);

}
std::list<MedMedicationDefinitionDO> selectWithPage(const DrugQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT * FROM Med_Medication_Defintion ";

    // 拼接条件
    sql += queryConditionBuilder(query, params);

    // 拼接排序：优先按更新时间降序，其次按创建时间，最后按ID
    sql += " ORDER BY IFNULL(`updated_at`, `created_at`) DESC, `id` DESC ";

    // 拼接分页
    sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

    // 执行查询并通过 Mapper 映射结果
    return sqlSession->executeQuery<MedMedicationDefinitionDO>(sql, PtrDrugMapper(), params);
}
// 通过姓名查询数据
std::list<MedMedicationDefinitionDO> selectByName(const string& name)
{

}
// 通过ID查询数据
MedMedicationDefinitionDO selectById(std::string id)
{

}
// 新增的数据插入 (供 saveData 调用)
int insert(const MedMedicationDefinitionDO& entity)
{
    std::string sql = "INSERT INTO Med_Medication_Definition"
        "(`id`, `name`, `category_code`, `inventory_status_enum`, `created_by`, `created_at`, `is_deleted`, `version`) "
        "VALUES (?, ?, ?, ?, ?, ?, 0, 0)";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, entity.getId());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getTenantId());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getCreatedAt());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedAt());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getCreatedBy());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getUpdatedBy());
    SQLPARAMS_PUSH(params, "b", bool, entity.getIsDeleted());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDeletedAt());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDeletedBy());
    SQLPARAMS_PUSH(params, "i", int, entity.getVersion());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getBusNo());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getName());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDomainEnum());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMedVersion());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getNameEn());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getPyStr());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getWbStr());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getCategoryCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMerchandiseName());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMerchandisePyStr());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMerchandiseWbStr());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getPurchaseUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getSplitUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getComprisedText());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getIngredient());
    SQLPARAMS_PUSH(params, "i", int, entity.getDoseFrom());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getApprovalNumber());
    SQLPARAMS_PUSH(params, "b", bool, entity.getYbMatchFlag());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getYbNo());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getPharmacologyCategoryCode());
    SQLPARAMS_PUSH(params, "b", bool, entity.getSkinTestFlag());
    SQLPARAMS_PUSH(params, "b", bool, entity.getInjectFlag());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getManufacturerId());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getManufacturerText());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getSupplyId());
    SQLPARAMS_PUSH(params, "b", bool, entity.getRestrictedFlag());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getRestrictedScope());
    SQLPARAMS_PUSH(params, "b", bool, entity.getActiveFlag());
    SQLPARAMS_PUSH(params, "b", bool, entity.getChildrenFlag());
    SQLPARAMS_PUSH(params, "i", int, entity.getCharacteristic());
    SQLPARAMS_PUSH(params, "d", double, entity.getItemMinQuantity());
    SQLPARAMS_PUSH(params, "d", double, entity.getItemMaxQuantity());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDefEncounterUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDefInhospitalUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getNationalDrugCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getPartAttributeEnum());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getAntibioticCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getRestrictedEnum());
    SQLPARAMS_PUSH(params, "b", bool, entity.getSelfFlag());
    SQLPARAMS_PUSH(params, "b", bool, entity.getAntibioticFlag());
    SQLPARAMS_PUSH(params, "b", bool, entity.getBasicFlag());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getThoPartAttributeEnum());
    SQLPARAMS_PUSH(params, "i", int, entity.getChrgitmLv());
    SQLPARAMS_PUSH(params, "b", bool, entity.getRxFlag());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDosageInstruction());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDrug69Code());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getTotalVolume());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDoseFormCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDoseUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getPackageUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getStockUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMethodCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getRateCode());
    SQLPARAMS_PUSH(params, "d", double, entity.getDose());
    SQLPARAMS_PUSH(params, "d", double, entity.getMaxUnit());
    SQLPARAMS_PUSH(params, "d", double, entity.getUsageLimit());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getLocationId());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getStatusEnum());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getOrgId());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getIngredientItem());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMedicationDefinition());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDddCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getDddUnitCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMinRateCode());
    SQLPARAMS_PUSH(params, "s", std::string, entity.getMaxRateCode());
    SQLPARAMS_PUSH(params, "d", double, entity.getDoseUnitRatio());
    SQLPARAMS_PUSH(params, "d", double, entity.getPurchaseUnitRatio());
    SQLPARAMS_PUSH(params, "d", double, entity.getStockUnitRatio());
    SQLPARAMS_PUSH(params, "d", double, entity.getPackageUnitRatio());

    return sqlSession->executeUpdate(sql, params);

}

