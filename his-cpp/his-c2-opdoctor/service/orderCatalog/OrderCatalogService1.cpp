#include "stdafx.h"
#include "OrderCatalogService1.h"
#include "domain/dto/orderCatalog/OrderCatalogItemDTO1.h"
#include "domain/dto/PageDTO.h"
#include "service/ClientManager.h"
#include "RedisClient.h"
#include "MongoClient.h"

OrderCatalogService1::OrderCatalogService1(std::shared_ptr<RedisClient> redisClient,
    std::shared_ptr<MongoClient> mongoClient)
    : m_redisClient(redisClient), m_mongoClient(mongoClient)
{
    m_sessionService = std::make_shared<SessionService>(redisClient);
    m_enumDAO = std::make_shared<EnumDAO>(mongoClient);
}

OrderCatalogService1::ServiceContext OrderCatalogService1::buildContext(const PayloadDTO& payload) {
    ServiceContext ctx;
    ctx.valid = false;
    if (payload.getId().empty()) return ctx;

    ctx.userId = payload.getId();
    auto orgIdOpt = m_sessionService->getOrgId(ctx.userId);
    ctx.orgId = orgIdOpt.has_value() ? orgIdOpt.value() : "";
    ctx.valid = true;
    return ctx;
}

OrderCatalogPageJsonVO1::Wrapper OrderCatalogService1::queryCatalogList(
    const OrderCatalogQuery1::Wrapper& query,
    const PayloadDTO& payload)
{
    auto ctx = buildContext(payload);
    if (!ctx.valid) {
        auto jvo = OrderCatalogPageJsonVO1::createShared();
        jvo->setStatus(RS_FAIL);
        jvo->message = oatpp::String("User context invalid");
        return jvo;
    }
    auto pageDto = OrderCatalogPageDTO1::createShared();
    pageDto->pageIndex = query->pageIndex;
    pageDto->pageSize = query->pageSize;

    std::string keyword = query->keyword ? query->keyword.getValue("") : "";
    std::string category = query->category ? query->category.getValue("") : "";

    uint64_t total = 0;
    auto items = m_dao.queryCatalogList(keyword, category,
        query->pageIndex, query->pageSize,
        ctx.orgId, total);

    auto rows = oatpp::List<OrderCatalogItemDTO1::Wrapper>::createShared();
    for (const auto& item : items) {
        auto dto = OrderCatalogItemDTO1::createShared();
        dto->id = oatpp::String(item.id);
        dto->code = oatpp::String(item.code);
        dto->name = oatpp::String(item.name);
        dto->category = oatpp::String(item.category);
        dto->categoryName = oatpp::String("");
        dto->specification = oatpp::String(item.specification);
        dto->defaultDose = oatpp::Float64(item.defaultDose);
        dto->defaultUnit = oatpp::String(item.defaultUnit);
        dto->defaultFrequency = oatpp::String(item.defaultFrequency);
        dto->defaultRoute = oatpp::String(item.defaultRoute);
        dto->defaultDays = oatpp::Int32(item.defaultDays);
        dto->defaultQuantity = oatpp::Float64(item.defaultQuantity);
        dto->quantityUnit = oatpp::String(item.quantityUnit);
        dto->dosePerQuantityUnit = oatpp::Float64(item.dosePerQuantityUnit);
        rows->push_back(dto);
    }

    pageDto->total = total;
    pageDto->calcPages();
    pageDto->rows = rows;

    auto jvo = OrderCatalogPageJsonVO1::createShared();
    jvo->success(pageDto);
    return jvo;
}