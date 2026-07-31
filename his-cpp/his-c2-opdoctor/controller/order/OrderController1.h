// controller/order/OrderController1.h
#pragma once
#ifndef _ORDER_CONTROLLER_1_H_
#define _ORDER_CONTROLLER_1_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/order/OrderQuery1.h"
#include "domain/vo/order/OrderVO1.h"
#include "domain/dto/order/OrderAddDTO1.h"
#include "domain/dto/order/OrderDetailDTO1.h"
#include "domain/dto/order/OrderModifyDTO1.h"
#include "domain/dto/PayloadDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("createOrder.order.tags.tag")

/**
 * 医嘱管理控制器
 * 提供医嘱的查询、新增、修改接口
 *
 * 重构说明：
 * - 医生ID (requester_id) 从 payload->getPractitionerId() 获取
 * - 医生姓名 从 payload->getPractitionerName() 获取
 * - tenantId/orgId 从 Redis user:session:{userId} 获取
 */
class OrderController1 : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(OrderController1);
public:
    // ========== 1. 分页查询医嘱 ==========
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("createOrder.order.query.summary"),
        queryOrder,
        OrderQuery1,
        OrderDetailPageJsonVO1::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/createOrder/query",
        queryOrder,
        OrderQuery1,
        execQueryOrder(query, authObject->getPayload())
    );

    // ========== 2. 新增医嘱 ==========
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("createOrder.order.add.summary"),
        addOrder,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/createOrder/add",
        addOrder,
        BODY_DTO(OrderAddDTO1::Wrapper, dto),
        execAddOrder(dto, authObject->getPayload())
    );

    // ========== 3. 修改医嘱 ==========
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("createOrder.order.modify.summary"),
        modifyOrder,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/createOrder/modify",
        modifyOrder,
        BODY_DTO(OrderModifyDTO1::Wrapper, dto),
        execModifyOrder(dto, authObject->getPayload())
    );


private:
    OrderDetailPageJsonVO1::Wrapper execQueryOrder(const OrderQuery1::Wrapper& query,
        const PayloadDTO& payload);
    StringJsonVO::Wrapper execAddOrder(const OrderAddDTO1::Wrapper& dto,
        const PayloadDTO& payload);
    StringJsonVO::Wrapper execModifyOrder(const OrderModifyDTO1::Wrapper& dto,
        const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif