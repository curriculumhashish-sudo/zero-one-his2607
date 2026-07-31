// controller/order/OrderController1.cpp
#include "stdafx.h"
#include "OrderController1.h"
#include "../../service/order/OrderService1.h"
#include "../../service/ClientManager.h"

OrderDetailPageJsonVO1::Wrapper OrderController1::execQueryOrder(
    const OrderQuery1::Wrapper& query,
    const PayloadDTO& payload)
{
    auto service = OrderService1(
        ClientManager::getRedisClient(),
        ClientManager::getMongoClient()
    );
    auto result = service.queryOrderList(query, &payload);
    auto jvo = OrderDetailPageJsonVO1::createShared();
    jvo->success(result);
    return jvo;
}

StringJsonVO::Wrapper OrderController1::execAddOrder(
    const OrderAddDTO1::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto jvo = StringJsonVO::createShared();

    if (!dto->orderType || dto->orderType->empty()) {
        jvo->init("orderType is required", RS_PARAMS_INVALID);
        return jvo;
    }

    auto service = OrderService1(
        ClientManager::getRedisClient(),
        ClientManager::getMongoClient()
    );

    std::string errorMsg;
    std::string id = service.addOrder(dto, &payload, errorMsg);

    if (!id.empty()) {
        jvo->success(id);
    }
    else {
        jvo->init(errorMsg.empty() ? "Failed to add order" : errorMsg, RS_FAIL);
    }
    return jvo;
}

StringJsonVO::Wrapper OrderController1::execModifyOrder(
    const OrderModifyDTO1::Wrapper& dto,
    const PayloadDTO& payload)
{
    auto jvo = StringJsonVO::createShared();

    if (!dto->orderId || dto->orderId->empty()) {
        jvo->init("orderId is required", RS_PARAMS_INVALID);
        return jvo;
    }

    auto service = OrderService1(
        ClientManager::getRedisClient(),
        ClientManager::getMongoClient()
    );

    std::string errorMsg;
    bool res = service.modifyOrder(dto, &payload, errorMsg);

    if (res) {
        jvo->success(dto->orderId);
    }
    else {
        jvo->init(errorMsg.empty() ? "Failed to modify order" : errorMsg, RS_FAIL);
    }
    return jvo;
}