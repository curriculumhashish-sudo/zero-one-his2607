#pragma once
#ifndef _ORDER_CATALOG_QUERY_1_H_
#define _ORDER_CATALOG_QUERY_1_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class OrderCatalogQuery1 : public PageQuery {
    DTO_INIT(OrderCatalogQuery1, PageQuery);

    API_DTO_FIELD_REQUIRE(
        String,
        keyword,
        ZH_WORDS_GETTER("createOrder.orderCatalog.field.keyword"),
        false
    );
    API_DTO_FIELD_REQUIRE(
        String,
        category,
        ZH_WORDS_GETTER("createOrder.orderCatalog.field.category"),
        false
    );
};

#include OATPP_CODEGEN_END(DTO)
#endif