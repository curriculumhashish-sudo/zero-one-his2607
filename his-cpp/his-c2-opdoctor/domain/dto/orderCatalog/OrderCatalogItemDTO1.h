#pragma once
#ifndef _ORDER_CATALOG_ITEM_DTO_1_H_
#define _ORDER_CATALOG_ITEM_DTO_1_H_

#include "domain/GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class OrderCatalogItemDTO1 : public oatpp::DTO {
    DTO_INIT(OrderCatalogItemDTO1, DTO);

    API_DTO_FIELD_REQUIRE(String, id, "Item ID", true);
    API_DTO_FIELD_REQUIRE(String, code, "Item code", true);
    API_DTO_FIELD_REQUIRE(String, name, "Item name", true);
    API_DTO_FIELD_REQUIRE(String, category, "Category code (DRUG/LAB/EXAM/TREATMENT)", true);
    API_DTO_FIELD_REQUIRE(String, categoryName, "Category display name", false);
    API_DTO_FIELD_REQUIRE(String, specification, "Specification", false);
    API_DTO_FIELD_REQUIRE(Float64, defaultDose, "Default dose per administration", false);
    API_DTO_FIELD_REQUIRE(String, defaultUnit, "Default dose unit", false);
    API_DTO_FIELD_REQUIRE(String, defaultFrequency, "Default frequency code", false);
    API_DTO_FIELD_REQUIRE(String, defaultRoute, "Default route code", false);
    API_DTO_FIELD_REQUIRE(Int32, defaultDays, "Default course days", false);
    API_DTO_FIELD_REQUIRE(Float64, defaultQuantity, "Default order quantity", true);
    API_DTO_FIELD_REQUIRE(String, quantityUnit, "Order quantity unit", true);
    API_DTO_FIELD_REQUIRE(Float64, dosePerQuantityUnit, "Dose per quantity unit", false);
};

#include OATPP_CODEGEN_END(DTO)
#endif