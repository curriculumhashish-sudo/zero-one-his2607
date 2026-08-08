#pragma once
#ifndef _ORDER_CATALOG_SERVICE_1_H_
#define _ORDER_CATALOG_SERVICE_1_H_

#include "domain/query/orderCatalog/OrderCatalogQuery1.h"
#include "domain/vo/orderCatalog/OrderCatalogPageJsonVO1.h"
#include "domain/dto/PayloadDTO.h"
#include "dao/orderCatalog/OrderCatalogDAO1.h"
#include "service/session/SessionService.h"
#include "dao/order/EnumDAO.h"
#include <memory>

class RedisClient;
class MongoClient;

class OrderCatalogService1 {
public:
    OrderCatalogService1(std::shared_ptr<RedisClient> redisClient,
        std::shared_ptr<MongoClient> mongoClient);

    OrderCatalogPageJsonVO1::Wrapper queryCatalogList(
        const OrderCatalogQuery1::Wrapper& query,
        const PayloadDTO& payload
    );

private:
    std::shared_ptr<RedisClient> m_redisClient;
    std::shared_ptr<MongoClient> m_mongoClient;
    std::shared_ptr<SessionService> m_sessionService;
    std::shared_ptr<EnumDAO> m_enumDAO;
    OrderCatalogDAO1 m_dao;

    struct ServiceContext {
        std::string userId;
        std::string orgId;
        bool valid;
    };
    ServiceContext buildContext(const PayloadDTO& payload);
};

#endif