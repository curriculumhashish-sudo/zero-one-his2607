#pragma once

#ifndef _PAGE_TCM_HERB_CATALOG_DO_H_
#define _PAGE_TCM_HERB_CATALOG_DO_H_

#include <string>

struct PageTcmHerbCatalogDO
{
    std::string id;
    std::string code;
    std::string name;
    std::string specification;
    double defaultDose = 0.0;
    std::string doseUnitCode;
    double retailPrice = 0.0;
    std::string categoryCode;
    double inventoryQuantity = 0.0;
    std::string inventoryUnitCode;
};

struct PageTcmHerbCatalogContextDO
{
    bool found = false;
    std::string tenantId;
    std::string hospitalOrganizationId;
};

#endif
