#include "stdafx.h"
#include "OrderCatalogDAO1.h"
#include "../../domain/do/order/MedicationDefinitionDO1.h"
#include "../../domain/do/order/ActivityDefinitionDO1.h"
#include "../order/Mapper/MedicationDefinitionMapper1.h"
#include "../order/Mapper/ActivityDefinitionMapper1.h"
#include <algorithm>

static std::string buildLikeClause(const std::string& keyword) {
    if (keyword.empty()) return "";
    return "%" + keyword + "%";
}

std::list<OrderCatalogDAO1::CatalogItem> OrderCatalogDAO1::queryCatalogList(
    const std::string& keyword,
    const std::string& category,
    uint64_t pageIndex,
    uint64_t pageSize,
    const std::string& orgId,
    uint64_t& total)
{
    std::list<CatalogItem> allItems;

    // ========== 1. 查询药品（med_medication_definition）==========
    bool queryDrug = category.empty() || category == "DRUG";
    if (queryDrug) {
        std::string sql = "SELECT * FROM med_medication_definition "
            "WHERE status_enum = 'ACTIVE' AND is_deleted = 0 AND org_id = ?";
        SqlParams params;
        SQLPARAMS_PUSH(params, "s", std::string, orgId);

        if (!keyword.empty()) {
            sql += " AND (bus_no LIKE ? OR name LIKE ? OR py_str LIKE ? OR wb_str LIKE ? OR merchandise_name LIKE ?)";
            std::string like = buildLikeClause(keyword);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
        }

        auto results = sqlSession->executeQuery<MedicationDefinitionDO1>(sql, MedicationDefinitionMapper1(), params);

        for (const auto& doObj : results) {
            CatalogItem item;
            item.id = doObj.getId();
            item.code = doObj.getBusNo();
            item.name = doObj.getName();
            item.category = "DRUG";
            item.specification = doObj.getTotalVolume();
            item.defaultDose = doObj.getDose();
            item.defaultUnit = doObj.getDoseUnitCode();
            item.defaultFrequency = doObj.getRateCode();
            item.defaultRoute = doObj.getMethodCode();
            item.defaultDays = 7;
            item.defaultQuantity = 1.0;
            item.quantityUnit = doObj.getPackageUnitCode();
            item.dosePerQuantityUnit = doObj.getDose();
            allItems.push_back(item);
        }
    }

    // ========== 2. 查询诊疗活动（wkf_activity_definition）==========
    bool queryActivity = category.empty() || category == "LAB" || category == "EXAM" || category == "TREATMENT";
    if (queryActivity) {
        std::string typeCondition;
        if (category == "LAB") {
            typeCondition = " AND type_enum = '22'";
        }
        else if (category == "EXAM") {
            typeCondition = " AND type_enum = '23'";
        }
        else if (category == "TREATMENT") {
            typeCondition = " AND type_enum NOT IN ('22', '23')";
        }

        std::string sql = "SELECT * FROM wkf_activity_definition "
            "WHERE status_enum = 'ACTIVE' AND is_deleted = 0 AND org_id = ?" + typeCondition;
        SqlParams params;
        SQLPARAMS_PUSH(params, "s", std::string, orgId);

        if (!keyword.empty()) {
            sql += " AND (bus_no LIKE ? OR name LIKE ? OR py_str LIKE ? OR wb_str LIKE ? OR merchandise_name LIKE ?)";
            std::string like = buildLikeClause(keyword);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
            SQLPARAMS_PUSH(params, "s", std::string, like);
        }

        auto results = sqlSession->executeQuery<ActivityDefinitionDO1>(sql, ActivityDefinitionMapper1(), params);

        for (const auto& doObj : results) {
            CatalogItem item;
            item.id = doObj.getId();
            item.code = doObj.getBusNo();
            item.name = doObj.getName();

            std::string type = doObj.getTypeEnum();
            if (type == "22") {
                item.category = "LAB";
            }
            else if (type == "23") {
                item.category = "EXAM";
            }
            else {
                item.category = "TREATMENT";
            }

            // 如果 category 不为空且与当前项不匹配，跳过
            if (!category.empty() && category != item.category) {
                continue;
            }

            item.specification = doObj.getDescriptionText();
            item.defaultDose = 0;
            item.defaultUnit = "";
            item.defaultFrequency = "";
            item.defaultRoute = "";
            item.defaultDays = 1;
            item.defaultQuantity = 1.0;
            item.quantityUnit = doObj.getPackageUnitCode();
            item.dosePerQuantityUnit = 0;
            allItems.push_back(item);
        }
    }

    // ========== 3. 排序（按编码升序）==========
    allItems.sort([](const CatalogItem& a, const CatalogItem& b) {
        return a.code < b.code;
        });

    total = allItems.size();

    // ========== 4. 分页截取 ==========
    uint64_t start = (pageIndex - 1) * pageSize;
    uint64_t end = std::min(start + pageSize, total);

    std::list<CatalogItem> pagedItems;
    uint64_t idx = 0;
    for (auto it = allItems.begin(); it != allItems.end() && idx < end; ++it, ++idx) {
        if (idx >= start) {
            pagedItems.push_back(*it);
        }
    }

    return pagedItems;
}