#include "stdafx.h"
#include "OrderController1.h"
#include "../../service/order/OrderService1.h"

OrderDetailPageJsonVO1::Wrapper OrderController1::execQueryOrder(const OrderQuery1::Wrapper& query)
{
    auto result = OrderService1().queryOrderList(query);
    auto jvo = OrderDetailPageJsonVO1::createShared();
    jvo->success(result);
    return jvo;
}

StringJsonVO::Wrapper OrderController1::execAddOrder(const OrderAddDTO1::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = StringJsonVO::createShared();

    // 参数校验
    if (!dto->orderType || dto->orderType->empty()) {
        jvo->init("orderType is required", RS_PARAMS_INVALID);
        return jvo;
    }

    // 将 payload 设置到 dto 中（与 SampleController 保持一致）
    dto->setPayload(&payload);

    // 调用 Service
    std::string id = OrderService1().addOrder(dto);
    if (!id.empty()) {
        jvo->success(id);
    }
    else {
        jvo->fail("");
    }
    return jvo;
}

StringJsonVO::Wrapper OrderController1::execModifyOrder(const OrderModifyDTO1::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = StringJsonVO::createShared();

    // 参数校验
    if (!dto->orderId || dto->orderId->empty()) {
        jvo->init("orderId is required", RS_PARAMS_INVALID);
        return jvo;
    }

    // 将 payload 设置到 dto 中
    dto->setPayload(&payload);

    bool res = OrderService1().modifyOrder(dto);
    if (res) {
        jvo->success(dto->orderId);
    }
    else {
        jvo->fail(dto->orderId);
    }
    return jvo;
}