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
#include "../../domain/DrugConstants.h"
#include "domain/query/Drug/DrugSelectQuery.h"
#include <sstream>

static const std::string &drugTableName()
{
    static const std::string tableName = MedMedicationDefinitionDO().getTable();
    return tableName;
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

std::string DrugDAO::queryConditionBuilder(const DrugQuery::Wrapper &query, SqlParams &params)
{
    std::ostringstream sqlCondition;
    sqlCondition << " WHERE is_deleted=0";

    if (query->keywords && !query->keywords->empty())
    {
        const std::string keywords = query->keywords.getValue("");
        sqlCondition << " AND (name LIKE CONCAT('%',?,'%') OR bus_no LIKE CONCAT('%',?,'%')"
                        " OR py_str LIKE CONCAT('%',?,'%') OR wb_str LIKE CONCAT('%',?,'%'))";
        for (int index = 0; index < 4; ++index)
        {
            SQLPARAMS_PUSH(params, "s", std::string, keywords);
        }
    }
    if (query->name && !query->name->empty())
    {
        const std::string name = query->name.getValue("");
        sqlCondition << " AND name LIKE CONCAT('%',?,'%')";
        SQLPARAMS_PUSH(params, "s", std::string, name);
    }
    if (query->kind && !query->kind->empty())
    {
        const std::string kind = query->kind.getValue("");
        sqlCondition << " AND category_code=?";
        SQLPARAMS_PUSH(params, "s", std::string, kind);
    }
     if (query->kind && !query->kind->empty())
    {
        const std::string kind = query->kind.getValue("");
        sqlCondition << " AND category_code=?";
        SQLPARAMS_PUSH(params, "s", std::string, kind);
    }
    return sqlCondition.str();
}
std::string querySelectConditionBuilder(const DrugSelectQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream sqlCondition;
    sqlCondition << " WHERE is_deleted=0";

    if (query->proname && !query->proname->empty())
    {
        const std::string pron = query->proname.getValue("");
        sqlCondition << " AND (name LIKE CONCAT('%',?,'%')";
        for (int index = 0; index < 4; ++index)
        {
            SQLPARAMS_PUSH(params, "s", std::string, pron);
        }
    }
    if (query->Minjiliang && !query->Minjiliang->empty())
    {
        const std::string zuixiao = query->Minjiliang.getValue("");
        sqlCondition << " AND name LIKE CONCAT('%',?,'%')";
        SQLPARAMS_PUSH(params, "s", std::string, zuixiao);
    }
    if (query->Maxjiliang && !query->Maxjiliang->empty())
    {
        const std::string zuida = query->Maxjiliang.getValue("");
        sqlCondition << " AND category_code=?";
        SQLPARAMS_PUSH(params, "s", std::string, zuida);
    }
    if (query->yongfa && !query->yongfa->empty())
    {
        const std::string yongfa = query->yongfa.getValue("");
        sqlCondition << " AND category_code=?";
        SQLPARAMS_PUSH(params, "s", std::string, yongfa);
    }
    return sqlCondition.str();


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
        placeholders(ids.size()) + ") AND is_deleted=0 ORDER BY id FOR UPDATE";
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
           "AND status_enum<>?";
    SQLPARAMS_PUSH(params, "s", std::string, data.getId());
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "i", int, expectedVersion);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::STATUS_DISCARDED);
    return sqlSession->executeUpdate(sql.str(), params);
}

int DrugDAO::softDeleteBasePrices(const std::string &drugId, const std::string &tenantId,
                                  const std::string &operatorId,
                                  const std::string &operationTime)
{
    const std::string sql =
        "UPDATE adm_charge_item_definition SET is_deleted=1,deleted_at=?,deleted_by=?,updated_at=?,updated_by=?,"
        "version=version+1 WHERE instance_table=? AND instance_id=? AND tenant_id=? "
        "AND is_deleted=0 "
        "AND lot_no IS NULL AND price_type_code IN (?,?,?)";
    SqlParams params;
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, drugTableName());
    SQLPARAMS_PUSH(params, "s", std::string, drugId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::PRICE_TYPE_PURCHASE);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::PRICE_TYPE_SALE);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::PRICE_TYPE_LIMIT);
    return sqlSession->executeUpdate(sql, params);
}

int DrugDAO::insertBasePrice(const AdmChargeItemDefinitionDO &price)
{
    return BaseDAO::insert(price);
}

std::list<PtrAdmChargeItemDefinitionDO> DrugDAO::selectBasePrices(const std::string &drugId,
                                                                 const std::string &tenantId)
{
    const std::string sql =
        "SELECT price_type_code,price_value,type_code,yb_type FROM adm_charge_item_definition "
        "WHERE instance_table=? AND instance_id=? AND tenant_id=? AND is_deleted=0 "
        "AND lot_no IS NULL AND price_type_code IN (?,?,?)";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, drugTableName());
    SQLPARAMS_PUSH(params, "s", std::string, drugId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::PRICE_TYPE_PURCHASE);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::PRICE_TYPE_SALE);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::PRICE_TYPE_LIMIT);
    return sqlSession->executeQuery<PtrAdmChargeItemDefinitionDO>(sql, PtrDrugPriceMapper(), params);
}

PtrDrugModifyTextResult DrugDAO::selectModifyResultTexts(const std::string &drugId,
                                                         const std::string &tenantId,
                                                         const std::string &typeCode)
{
    const std::string sql =
        "SELECT COALESCE(purchase_unit.display,''),COALESCE(split_unit.display,''),"
        "COALESCE(dose_unit.display,''),COALESCE(package_unit.display,''),"
        "COALESCE(stock_unit.display,''),COALESCE(dose_from.display,''),"
        "COALESCE(antibiotic.display,''),COALESCE(fin_type.display,''),"
        "COALESCE(category.display,''),COALESCE(location.name,''),COALESCE(supplier.name,'') "
        "FROM med_medication_definition d "
        "LEFT JOIN ter_code_system unit_system ON unit_system.name='unit_code' "
        "AND unit_system.is_deleted=0 AND unit_system.status IN ('ACTIVE','2') "
        "AND (unit_system.tenant_id=d.tenant_id OR unit_system.tenant_id IS NULL) "
        "LEFT JOIN ter_concept purchase_unit ON purchase_unit.code_system_id=unit_system.id "
        "AND purchase_unit.code=d.purchase_unit_code AND purchase_unit.is_deleted=0 "
        "AND purchase_unit.is_active=1 "
        "AND (purchase_unit.tenant_id=d.tenant_id OR purchase_unit.tenant_id IS NULL) "
        "LEFT JOIN ter_concept split_unit ON split_unit.code_system_id=unit_system.id "
        "AND split_unit.code=d.split_unit_code AND split_unit.is_deleted=0 AND split_unit.is_active=1 "
        "AND (split_unit.tenant_id=d.tenant_id OR split_unit.tenant_id IS NULL) "
        "LEFT JOIN ter_concept dose_unit ON dose_unit.code_system_id=unit_system.id "
        "AND dose_unit.code=d.dose_unit_code AND dose_unit.is_deleted=0 AND dose_unit.is_active=1 "
        "AND (dose_unit.tenant_id=d.tenant_id OR dose_unit.tenant_id IS NULL) "
        "LEFT JOIN ter_concept package_unit ON package_unit.code_system_id=unit_system.id "
        "AND package_unit.code=d.package_unit_code AND package_unit.is_deleted=0 AND package_unit.is_active=1 "
        "AND (package_unit.tenant_id=d.tenant_id OR package_unit.tenant_id IS NULL) "
        "LEFT JOIN ter_concept stock_unit ON stock_unit.code_system_id=unit_system.id "
        "AND stock_unit.code=d.stock_unit_code AND stock_unit.is_deleted=0 AND stock_unit.is_active=1 "
        "AND (stock_unit.tenant_id=d.tenant_id OR stock_unit.tenant_id IS NULL) "
        "LEFT JOIN ter_code_system dose_from_system ON dose_from_system.name='dose_from_code' "
        "AND dose_from_system.is_deleted=0 AND dose_from_system.status IN ('ACTIVE','2') "
        "AND (dose_from_system.tenant_id=d.tenant_id OR dose_from_system.tenant_id IS NULL) "
        "LEFT JOIN ter_concept dose_from ON dose_from.code_system_id=dose_from_system.id "
        "AND dose_from.code=CAST(d.dose_from AS CHAR CHARACTER SET utf8mb4) COLLATE utf8mb4_general_ci "
        "AND dose_from.is_deleted=0 AND dose_from.is_active=1 "
        "AND (dose_from.tenant_id=d.tenant_id OR dose_from.tenant_id IS NULL) "
        "LEFT JOIN ter_code_system antibiotic_system ON antibiotic_system.name='antibiotic_type_code' "
        "AND antibiotic_system.is_deleted=0 AND antibiotic_system.status IN ('ACTIVE','2') "
        "AND (antibiotic_system.tenant_id=d.tenant_id OR antibiotic_system.tenant_id IS NULL) "
        "LEFT JOIN ter_concept antibiotic ON antibiotic.code_system_id=antibiotic_system.id "
        "AND antibiotic.code=d.antibiotic_code AND antibiotic.is_deleted=0 AND antibiotic.is_active=1 "
        "AND (antibiotic.tenant_id=d.tenant_id OR antibiotic.tenant_id IS NULL) "
        "LEFT JOIN ter_code_system fin_type_system ON fin_type_system.name='fin_type_code' "
        "AND fin_type_system.is_deleted=0 AND fin_type_system.status IN ('ACTIVE','2') "
        "AND (fin_type_system.tenant_id=d.tenant_id OR fin_type_system.tenant_id IS NULL) "
        "LEFT JOIN ter_concept fin_type ON fin_type.code_system_id=fin_type_system.id "
        "AND fin_type.code=? AND fin_type.is_deleted=0 AND fin_type.is_active=1 "
        "AND (fin_type.tenant_id=d.tenant_id OR fin_type.tenant_id IS NULL) "
        "LEFT JOIN ter_code_system category_system ON category_system.name='medication_category_code' "
        "AND category_system.is_deleted=0 AND category_system.status IN ('ACTIVE','2') "
        "AND (category_system.tenant_id=d.tenant_id OR category_system.tenant_id IS NULL) "
        "LEFT JOIN ter_concept category ON category.code_system_id=category_system.id "
        "AND category.code=d.category_code AND category.is_deleted=0 AND category.is_active=1 "
        "AND (category.tenant_id=d.tenant_id OR category.tenant_id IS NULL) "
        "LEFT JOIN adm_location location ON location.id=d.location_id AND location.is_deleted=0 "
        "AND (location.tenant_id=d.tenant_id OR location.tenant_id IS NULL) "
        "LEFT JOIN cus_supplier supplier ON supplier.id=d.supply_id AND supplier.is_deleted=0 "
        "AND (supplier.tenant_id=d.tenant_id OR supplier.tenant_id IS NULL) "
        "WHERE d.id=? AND d.tenant_id=? AND d.is_deleted=0 LIMIT 1";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, typeCode);
    SQLPARAMS_PUSH(params, "s", std::string, drugId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    return sqlSession->executeQueryOne<PtrDrugModifyTextResult>(sql, PtrDrugModifyTextMapper(), params);
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
        "EXISTS(SELECT id FROM wkf_inventory_item WHERE item_table=? "
        "AND item_id IN (" +
        idPlaceholders +
        ") AND is_deleted=0 AND inventory_status_enum=? AND quantity<>0 LIMIT 1) "
        "THEN 1 ELSE 0 END";

    SqlParams params;
    appendIds(params, ids);
    appendIds(params, ids);
    appendIds(params, ids);
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, drugTableName());
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::STATUS_ACTIVE);
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
        "WHERE p.instance_table=? AND p.instance_id=d.id "
        "AND p.tenant_id=d.tenant_id AND p.is_deleted=0 AND p.status_enum=? AND p.lot_no IS NULL "
        "AND p.price_type_code=? AND p.price_value>=0 AND TRIM(COALESCE(p.type_code,''))<>'' LIMIT 1) "
        "THEN 1 ELSE 0 END)=? THEN 1 ELSE 0 END "
        "FROM med_medication_definition d WHERE d.id IN (" +
        placeholders(ids.size()) +
        ") AND d.tenant_id=? AND d.is_deleted=0 AND d.status_enum=?";

    SqlParams params;
    SQLPARAMS_PUSH(params, "i", int, static_cast<int>(ids.size()));
    SQLPARAMS_PUSH(params, "s", std::string, drugTableName());
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::STATUS_ACTIVE);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::PRICE_TYPE_SALE);
    SQLPARAMS_PUSH(params, "i", int, static_cast<int>(ids.size()));
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::STATUS_INACTIVE);
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
        "WHERE instance_table=? AND instance_id IN (" +
        placeholders(ids.size()) +
        ") AND tenant_id=? AND is_deleted=0 LIMIT 1) THEN 1 ELSE 0 END";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, drugTableName());
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
        "UPDATE adm_charge_item_definition SET is_deleted=1,status_enum=?,deleted_at=?,deleted_by=?,"
        "updated_at=?,updated_by=?,version=version+1 WHERE instance_table=? "
        "AND instance_id IN (" +
        placeholders(ids.size()) + ") AND tenant_id=? AND is_deleted=0";
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::STATUS_INACTIVE);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, drugTableName());
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
        "UPDATE med_medication_definition SET is_deleted=1,active_flag=?,status_enum=?,"
        "deleted_at=?,deleted_by=?,updated_at=?,updated_by=?,version=version+1 "
        "WHERE id IN (" +
        placeholders(ids.size()) + ") AND tenant_id=? AND is_deleted=0 AND status_enum=?";
    SqlParams params;
    SQLPARAMS_PUSH(params, "b", bool, false);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::STATUS_DISCARDED);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "dt", std::string, operationTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    appendIds(params, ids);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, DrugConstants::STATUS_INACTIVE);
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
uint64_t DrugDAO::count(const DrugQuery::Wrapper &query)
{
    SqlParams params;
    std::string sql = "SELECT COUNT(*) FROM med_medication_definition";
    sql += queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PtrMedMedicationDefinitionDO> DrugDAO::selectWithPage(const DrugQuery::Wrapper &query)
{
    SqlParams params;
    std::string sql = "SELECT " + fullDrugColumns() + " FROM med_medication_definition";
    sql += queryConditionBuilder(query, params);
    sql += " ORDER BY IFNULL(updated_at,created_at) DESC,id DESC LIMIT ?,?";

    const uint64_t pageIndex = query->pageIndex.getValue(1);
    const uint64_t pageSize = query->pageSize.getValue(10);
    const uint64_t offset = (pageIndex - 1) * pageSize;
    SQLPARAMS_PUSH(params, "ull", uint64_t, offset);
    SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);
    return sqlSession->executeQuery<PtrMedMedicationDefinitionDO>(sql, PtrDrugMapper(), params);
}
std::list<PtrMedMedicationDefinitionDO> DrugDAO::SElectWithPage(const DrugSelectQuery::Wrapper& query)
{
    SqlParams params;
    std::string sql = "SELECT " + fullDrugColumns() + " FROM med_medication_definition";
    sql += querySelectConditionBuilder(query, params);
    sql += " ORDER BY IFNULL(updated_at,created_at) DESC,id DESC LIMIT ?,?";

    const uint64_t pageIndex = query->pageIndex.getValue(1);
    const uint64_t pageSize = query->pageSize.getValue(10);
    const uint64_t offset = (pageIndex - 1) * pageSize;
    SQLPARAMS_PUSH(params, "ull", uint64_t, offset);
    SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);
    return sqlSession->executeQuery<PtrMedMedicationDefinitionDO>(sql, PtrDrugMapper(), params);
}
int DrugDAO::insert(const MedMedicationDefinitionDO &entity)
{
    return BaseDAO::insert(entity);
}

