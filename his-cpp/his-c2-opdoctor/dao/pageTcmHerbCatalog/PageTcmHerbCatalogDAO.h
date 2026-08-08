#pragma once

#ifndef _PAGE_TCM_HERB_CATALOG_DAO_H_
#define _PAGE_TCM_HERB_CATALOG_DAO_H_

#include "../../domain/dto/pageTcmHerbCatalog/PageTcmHerbCatalogDTO.h"
#include "BaseDAO.h"

class PageTcmHerbCatalogDAO : public BaseDAO
{
public:
    PageTcmHerbCatalogContextDO selectUserContext(
        const std::string& userId);

    std::uint64_t countPageTcmHerbCatalog(
        const PageTcmHerbCatalogDTO& query);

    std::vector<PageTcmHerbCatalogDO> selectPageTcmHerbCatalog(
        const PageTcmHerbCatalogDTO& query);
};

#endif
