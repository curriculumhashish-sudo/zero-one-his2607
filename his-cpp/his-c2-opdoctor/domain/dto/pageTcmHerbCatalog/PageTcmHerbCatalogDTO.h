#pragma once

#ifndef _PAGE_TCM_HERB_CATALOG_DTO_H_
#define _PAGE_TCM_HERB_CATALOG_DTO_H_

#include "../../do/pageTcmHerbCatalog/PageTcmHerbCatalogDO.h"

#include <cstdint>
#include <string>
#include <vector>

struct PageTcmHerbCatalogDTO
{
    std::string scope;
    std::string keyword;
    std::string tenantId;
    std::string hospitalOrganizationId;
    std::string inventoryLocationName;
    std::uint64_t pageIndex = 1;
    std::uint64_t pageSize = 20;
    std::uint64_t offset = 0;
};

struct PageTcmHerbCatalogResultDTO
{
    std::uint64_t pageIndex = 1;
    std::uint64_t pageSize = 20;
    std::uint64_t total = 0;
    std::vector<PageTcmHerbCatalogDO> rows;
};

#endif
