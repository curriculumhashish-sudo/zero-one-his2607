#include "stdafx.h"
#include "PageTcmHerbCatalogService.h"

#include <algorithm>
#include <cctype>
#include <limits>

namespace
{
std::string trim(const std::string& value)
{
    const auto first = std::find_if_not(
        value.begin(), value.end(),
        [](unsigned char character) { return std::isspace(character); });
    const auto last = std::find_if_not(
        value.rbegin(), value.rend(),
        [](unsigned char character) { return std::isspace(character); })
        .base();
    return first < last ? std::string(first, last) : std::string();
}

std::string lowerAscii(std::string value)
{
    std::transform(
        value.begin(), value.end(), value.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(std::tolower(character));
        });
    return value;
}
}

PageTcmHerbCatalogResultDTO
PageTcmHerbCatalogService::pageTcmHerbCatalog(
    const PageTcmHerbCatalogQuery::Wrapper& query,
    const std::string& userId)
{
    if (!query)
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_INVALID_PARAMS,
            "Request body is required");
    }
    if (userId.empty())
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_UNAVAILABLE,
            "Authenticated user context is unavailable");
    }
    if (!query->pageIndex || *query->pageIndex < 1)
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_INVALID_PARAMS,
            "pageIndex must be at least 1");
    }
    if (!query->pageSize || *query->pageSize < 1 ||
        *query->pageSize > MAX_PAGE_SIZE)
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_INVALID_PARAMS,
            "pageSize must be between 1 and 100");
    }

    const std::string scope = query->scope
        ? lowerAscii(trim(*query->scope))
        : std::string();
    if (scope != "outpatient" && scope != "inpatient")
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_INVALID_PARAMS,
            "scope must be outpatient or inpatient");
    }

    const std::string keyword = query->keyword
        ? trim(*query->keyword)
        : std::string();
    if (keyword.size() > MAX_KEYWORD_LENGTH)
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_INVALID_PARAMS,
            "keyword cannot exceed 100 characters");
    }

    const std::uint64_t pageIndex = *query->pageIndex;
    const std::uint64_t pageSize = *query->pageSize;
    if (pageIndex - 1 >
        std::numeric_limits<std::uint64_t>::max() / pageSize)
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_INVALID_PARAMS,
            "Requested page is too large");
    }

    const auto context = dao_.selectUserContext(userId);
    if (!context.found)
    {
        throw PageTcmHerbCatalogBusinessException(
            PAGE_TCM_HERB_CATALOG_UNAVAILABLE,
            "Tenant context is unavailable for the authenticated user");
    }

    PageTcmHerbCatalogDTO normalized;
    normalized.scope = scope;
    normalized.keyword = keyword;
    normalized.tenantId = context.tenantId;
    normalized.hospitalOrganizationId =
        context.hospitalOrganizationId;
    normalized.inventoryLocationName = scope == "outpatient"
        ? "\xE9\x97\xA8" "\xE8\xAF\x8A" "\xE4\xB8\xAD"
          "\xE8\x8D\xAF" "\xE6\x88\xBF"
        : "\xE4\xBD\x8F" "\xE9\x99\xA2" "\xE4\xB8\xAD"
          "\xE8\x8D\xAF" "\xE6\x88\xBF";
    normalized.pageIndex = pageIndex;
    normalized.pageSize = pageSize;
    normalized.offset = (pageIndex - 1) * pageSize;

    PageTcmHerbCatalogResultDTO result;
    result.pageIndex = pageIndex;
    result.pageSize = pageSize;
    result.total = dao_.countPageTcmHerbCatalog(normalized);
    if (result.total > 0 && normalized.offset < result.total)
    {
        result.rows = dao_.selectPageTcmHerbCatalog(normalized);
    }
    return result;
}
