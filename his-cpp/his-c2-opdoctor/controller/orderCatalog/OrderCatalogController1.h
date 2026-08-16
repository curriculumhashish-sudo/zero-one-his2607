// controller/orderCatalog/OrderCatalogController1.h
#pragma once
#ifndef _ORDER_CATALOG_CONTROLLER_1_H_
#define _ORDER_CATALOG_CONTROLLER_1_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/orderCatalog/OrderCatalogPageJsonVO1.h"
#include "domain/query/orderCatalog/OrderCatalogQuery1.h"
#include "domain/dto/PayloadDTO.h"
#include "service/orderCatalog/OrderCatalogService1.h"
#include "service/ClientManager.h"
#include "ApiHelper.h"
#include "ServerInfo.h"
#include "YamlHelper.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("createOrder.orderCatalog.tag")

class OrderCatalogController1 : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(OrderCatalogController1);

public:
    // ========== 1. 分页查询医嘱项目目录 ==========
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("createOrder.orderCatalog.query.summary"),
        queryCatalog,
        OrderCatalogQuery1,
        OrderCatalogPageJsonVO1::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/c2-opdoctor/doctor-station/page-advice-order-catalog",
        queryCatalog,
        OrderCatalogQuery1,
        execQueryCatalog(query, authObject->getPayload())
    );

private:
    OrderCatalogPageJsonVO1::Wrapper execQueryCatalog(
        const OrderCatalogQuery1::Wrapper& query,
        const PayloadDTO& payload
    );
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif