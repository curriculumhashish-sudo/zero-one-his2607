#pragma once
#ifndef _ORDER_CATALOG_PAGE_JSON_VO_1_H_
#define _ORDER_CATALOG_PAGE_JSON_VO_1_H_

#include "domain/vo/JsonVO.h"
#include "domain/dto/PageDTO.h"
#include "domain/dto/orderCatalog/OrderCatalogItemDTO1.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrderCatalogPageDTO1 : public PageDTO<OrderCatalogItemDTO1::Wrapper> {
    DTO_INIT(OrderCatalogPageDTO1, PageDTO<OrderCatalogItemDTO1::Wrapper>);
};

class OrderCatalogPageJsonVO1 : public JsonVO<OrderCatalogPageDTO1::Wrapper> {
    DTO_INIT(OrderCatalogPageJsonVO1, JsonVO<OrderCatalogPageDTO1::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif