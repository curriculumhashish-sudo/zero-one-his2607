#include "stdafx.h"
#include "PageTcmHerbCatalogController.h"

#include <limits>

namespace
{
std::string displayUnit(const std::string& unitCode)
{
    if (unitCode == "10")
    {
        return "g";
    }
    return unitCode;
}

PageTcmHerbCatalogItemVO::Wrapper toItemVO(
    const PageTcmHerbCatalogDO& source)
{
    auto item = PageTcmHerbCatalogItemVO::createShared();
    item->id = source.id;
    item->code = source.code;
    item->name = source.name;
    item->specification = source.specification;
    item->defaultDose = source.defaultDose;
    item->doseUnit = displayUnit(source.doseUnitCode);
    item->retailPrice = source.retailPrice;
    item->categoryCode = source.categoryCode;
    item->category =
        "\xE4\xB8\xAD" "\xE8\x8D\x89" "\xE8\x8D\xAF";
    item->inventoryQuantity = source.inventoryQuantity;
    item->inventoryUnit = displayUnit(source.inventoryUnitCode);
    return item;
}

PageTcmHerbCatalogJsonVO::Wrapper makeResponse(
    int code,
    const std::string& message,
    std::uint64_t pageIndex = 1,
    std::uint64_t pageSize = 20,
    std::uint64_t total = 0)
{
    auto response = PageTcmHerbCatalogJsonVO::createShared();
    response->setStatus(ResultStatus(message, code));
    response->data->pageIndex = pageIndex;
    response->data->pageSize = pageSize;
    response->data->total = static_cast<v_int64>(total);
    response->data->calcPages();
    response->data->rows =
        oatpp::List<PageTcmHerbCatalogItemVO::Wrapper>::createShared();
    return response;
}
}

void PageTcmHerbCatalogController::addSwaggerExamples(
    const std::shared_ptr<EndpointInfo>& info)
{
    auto request = PageTcmHerbCatalogQuery::createShared();
    request->scope = "outpatient";
    request->pageIndex = 1;
    request->pageSize = 20;
    request->keyword = "\xE9\xBB\x84" "\xE8\x8A\xAA";
    info->body.description =
        ZH_WORDS_GETTER("pageTcmHerbCatalog.page.description");
    info->body.addExample("default", request);

    auto success = makeResponse(
        10000, "Operation successful", 1, 20, 1);
    auto item = PageTcmHerbCatalogItemVO::createShared();
    item->id = "323999010000000001";
    item->code = "MD0030";
    item->name = "\xE9\xBB\x84" "\xE8\x8A\xAA";
    item->specification = "\xE9\xA5\xAE" "\xE7\x89\x87";
    item->defaultDose = 10.0;
    item->doseUnit = "g";
    item->retailPrice = 0.085;
    item->categoryCode = "4";
    item->category =
        "\xE4\xB8\xAD" "\xE8\x8D\x89" "\xE8\x8D\xAF";
    item->inventoryQuantity = 5000.0;
    item->inventoryUnit = "g";
    success->data->rows->push_back(item);
    info->responses[Status::CODE_200].addExample("success", success);
    info->responses[Status::CODE_200].addExample(
        "invalidParams",
        makeResponse(
            PAGE_TCM_HERB_CATALOG_INVALID_PARAMS,
            "pageSize must be between 1 and 100"));
    info->responses[Status::CODE_200].addExample(
        "unavailable",
        makeResponse(
            PAGE_TCM_HERB_CATALOG_UNAVAILABLE,
            "Catalog service is unavailable"));
}

PageTcmHerbCatalogJsonVO::Wrapper
PageTcmHerbCatalogController::execPageTcmHerbCatalog(
    const PageTcmHerbCatalogQuery::Wrapper& query,
    const PayloadDTO& payload)
{
    try
    {
        PageTcmHerbCatalogService service;
        const auto result = service.pageTcmHerbCatalog(
            query, payload.getId());
        auto response = makeResponse(
            10000,
            "Operation successful",
            result.pageIndex,
            result.pageSize,
            result.total);
        for (const auto& row : result.rows)
        {
            response->data->rows->push_back(toItemVO(row));
        }
        return response;
    }
    catch (const PageTcmHerbCatalogBusinessException& exception)
    {
        return makeResponse(exception.code(), exception.what());
    }
    catch (const sql::SQLException&)
    {
        return makeResponse(
            PAGE_TCM_HERB_CATALOG_UNAVAILABLE,
            "Database operation failed");
    }
    catch (const std::exception&)
    {
        return makeResponse(
            PAGE_TCM_HERB_CATALOG_UNAVAILABLE,
            "Catalog service is unavailable");
    }
}
