#pragma once

#ifndef _PAGE_TCM_HERB_CATALOG_SERVICE_H_
#define _PAGE_TCM_HERB_CATALOG_SERVICE_H_

#include "../../dao/pageTcmHerbCatalog/PageTcmHerbCatalogDAO.h"
#include "../../domain/query/pageTcmHerbCatalog/PageTcmHerbCatalogQuery.h"

#include <stdexcept>

enum PageTcmHerbCatalogErrorCode
{
    PAGE_TCM_HERB_CATALOG_INVALID_PARAMS = 9995,
    PAGE_TCM_HERB_CATALOG_UNAVAILABLE = 9999
};

class PageTcmHerbCatalogBusinessException : public std::runtime_error
{
public:
    PageTcmHerbCatalogBusinessException(
        int code,
        const std::string& message)
        : std::runtime_error(message), code_(code)
    {
    }

    int code() const
    {
        return code_;
    }

private:
    int code_;
};

class PageTcmHerbCatalogService
{
public:
    PageTcmHerbCatalogResultDTO pageTcmHerbCatalog(
        const PageTcmHerbCatalogQuery::Wrapper& query,
        const std::string& userId);

private:
    static constexpr std::uint64_t MAX_PAGE_SIZE = 100;
    static constexpr std::size_t MAX_KEYWORD_LENGTH = 100;

    PageTcmHerbCatalogDAO dao_;
};

#endif
