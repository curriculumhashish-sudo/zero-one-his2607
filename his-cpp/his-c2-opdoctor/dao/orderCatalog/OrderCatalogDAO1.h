#pragma once
#ifndef _ORDER_CATALOG_DAO_1_H_
#define _ORDER_CATALOG_DAO_1_H_

#include "BaseDAO.h"
#include <list>
#include <string>

class OrderCatalogDAO1 : public BaseDAO {
public:
    struct CatalogItem {
        std::string id;
        std::string code;
        std::string name;
        std::string category;          // DRUG / LAB / EXAM / TREATMENT
        std::string specification;
        double defaultDose;
        std::string defaultUnit;
        std::string defaultFrequency;
        std::string defaultRoute;
        int defaultDays;
        double defaultQuantity;
        std::string quantityUnit;
        double dosePerQuantityUnit;
    };

    /**
     * 分页查询医嘱目录项目（药品 + 诊疗）
     * @param keyword     关键词（空表示不过滤）
     * @param category    分类过滤（DRUG/LAB/EXAM/TREATMENT，空表示全部）
     * @param pageIndex   页码（从1开始）
     * @param pageSize    每页大小
     * @param orgId       机构ID（用于过滤）
     * @param total       输出总记录数
     * @return 项目列表（已排序并分页）
     */
    std::list<CatalogItem> queryCatalogList(
        const std::string& keyword,
        const std::string& category,
        uint64_t pageIndex,
        uint64_t pageSize,
        const std::string& orgId,
        uint64_t& total
    );
};

#endif