// controller/orderCatalog/OrderCatalogController1.cpp
#include "stdafx.h"
#include "OrderCatalogController1.h"
#include "service/orderCatalog/OrderCatalogService1.h"
#include "service/ClientManager.h"
#include "domain/vo/BaseJsonVO.h"

// ========== 1. 分页查询医嘱项目目录 ==========
OrderCatalogPageJsonVO1::Wrapper OrderCatalogController1::execQueryCatalog(
    const OrderCatalogQuery1::Wrapper& query,
    const PayloadDTO& payload)
{
    auto service = OrderCatalogService1(
        ClientManager::getRedisClient(),
        ClientManager::getMongoClient()
    );
    auto jvo = service.queryCatalogList(query, payload);

    if (jvo && jvo->data && jvo->data->rows) {
        auto& rows = jvo->data->rows;
        for (auto& dto : *rows) {
            if (dto->category) {
                std::string cat = dto->category.getValue("");
                std::string key = "createOrder.orderCatalog.categoryNames." + cat;
                std::string display = YamlHelper().getString(
                    &ServerInfo::getInstance().getZhDictNode(),
                    key
                );
                if (display.empty()) display = cat;
                dto->categoryName = oatpp::String(display);
            }
        }
    }
    return jvo;
}