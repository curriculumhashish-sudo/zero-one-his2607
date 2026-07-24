#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _INPATIENT_BILLING_DTO_H_
#define _INPATIENT_BILLING_DTO_H_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// =======================================================================
// SECTION 1: Pricing Catalog (GET /inpatientbilling/page-pricing-catalog)
// =======================================================================

class PagePricingCatalogQuery : public PageQuery
{
    DTO_INIT(PagePricingCatalogQuery, DTO);

    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), false);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), false);
    API_DTO_FIELD_REQUIRE(String, keyword,
        ZH_WORDS_GETTER("inpatientbilling.field.keyword"), false);
    API_DTO_FIELD_REQUIRE(String, categoryCode,
        ZH_WORDS_GETTER("inpatientbilling.field.categoryCode"), false);
    API_DTO_FIELD_REQUIRE(String, organizationUnitId,
        ZH_WORDS_GETTER("inpatientbilling.field.organizationUnitId"), false);
};

class NurseStationPricingCatalogRowDTO : public oatpp::DTO
{
    DTO_INIT(NurseStationPricingCatalogRowDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, productDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.productDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(String, name,
        ZH_WORDS_GETTER("inpatientbilling.field.name"), false);
    API_DTO_FIELD_REQUIRE(Float64, unitPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.unitPrice"), false);
    API_DTO_FIELD_REQUIRE(String, categoryCode,
        ZH_WORDS_GETTER("inpatientbilling.field.categoryCode"), false);
    API_DTO_FIELD_REQUIRE(String, categoryCodeText,
        ZH_WORDS_GETTER("inpatientbilling.field.categoryCodeText"), false);
    API_DTO_FIELD_REQUIRE(String, unitCode,
        ZH_WORDS_GETTER("inpatientbilling.field.unitCode"), false);
    API_DTO_FIELD_REQUIRE(String, unitCodeText,
        ZH_WORDS_GETTER("inpatientbilling.field.unitCodeText"), false);
    API_DTO_FIELD_REQUIRE(String, specificationText,
        ZH_WORDS_GETTER("inpatientbilling.field.specificationText"), false);
    API_DTO_FIELD_REQUIRE(String, organizationUnitId,
        ZH_WORDS_GETTER("inpatientbilling.field.organizationUnitId"), false);
    API_DTO_FIELD_REQUIRE(String, organizationUnitName,
        ZH_WORDS_GETTER("inpatientbilling.field.organizationUnitName"), false);

public:
    NurseStationPricingCatalogRowDTO()
    {
        productDefinitionId = "334568157811871744";
        name = "替米沙坦片";
        unitPrice = 2.50;
        categoryCode = "DRUG";
        categoryCodeText = "药品";
        unitCode = "片";
        unitCodeText = "片";
        specificationText = "40mg*14片";
        organizationUnitId = "334526752078102528";
        organizationUnitName = "住院药房";
    }
};

class PagePricingCatalogDTO : public PageDTO<NurseStationPricingCatalogRowDTO::Wrapper>
{
    DTO_INIT(PagePricingCatalogDTO, PageDTO<NurseStationPricingCatalogRowDTO::Wrapper>);

public:
    PagePricingCatalogDTO()
    {
        this->pageIndex = 1;
        this->pageSize = 10;
        this->total = 1;
        this->pages = 1;
        this->rows = List<NurseStationPricingCatalogRowDTO::Wrapper>::createShared();
        this->rows->push_back(NurseStationPricingCatalogRowDTO::createShared());
    }
};

// =======================================================================
// SECTION 2: Confirm Direct Pricing (POST /inpatientbilling/confirm-direct-pricing)
// =======================================================================

class ConfirmDirectPricingItemDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmDirectPricingItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(Boolean, enabled,
        ZH_WORDS_GETTER("inpatientbilling.field.enabled"), true);
    API_DTO_FIELD_REQUIRE(String, accountId,
        ZH_WORDS_GETTER("inpatientbilling.field.accountId"), true);
    API_DTO_FIELD_REQUIRE(String, productTableName,
        ZH_WORDS_GETTER("inpatientbilling.field.productTableName"), true);
    API_DTO_FIELD_REQUIRE(String, productDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.productDefinitionId"), true);
    API_DTO_FIELD_REQUIRE(Float64, quantity,
        ZH_WORDS_GETTER("inpatientbilling.field.quantity"), true);
    API_DTO_FIELD_REQUIRE(String, unitCode,
        ZH_WORDS_GETTER("inpatientbilling.field.unitCode"), true);
    API_DTO_FIELD_REQUIRE(String, chargeItemDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.chargeItemDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(Float64, unitPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.unitPrice"), true);
    API_DTO_FIELD_REQUIRE(Float64, totalPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.totalPrice"), true);
    API_DTO_FIELD_REQUIRE(String, remark,
        ZH_WORDS_GETTER("inpatientbilling.field.remark"), false);

public:
    ConfirmDirectPricingItemDTO()
    {
        encounterId = "337907884548657152";
        patientId = "326940849401761792";
        enabled = true;
        accountId = "337907884548657153";
        productTableName = "product_table";
        productDefinitionId = "334568157811871744";
        quantity = 2.0;
        unitCode = "片";
        chargeItemDefinitionId = "334568157811871745";
        unitPrice = 2.50;
        totalPrice = 5.00;
        remark = "Direct charge item";
    }
};

class ConfirmDirectPricingDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmDirectPricingDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<ConfirmDirectPricingItemDTO::Wrapper>, items,
        ZH_WORDS_GETTER("inpatientbilling.field.items"), true);

public:
    ConfirmDirectPricingDTO()
    {
        items = List<ConfirmDirectPricingItemDTO::Wrapper>::createShared();
        items->push_back(ConfirmDirectPricingItemDTO::createShared());
    }
};

class ConfirmDirectPricingResponseDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmDirectPricingResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);
    API_DTO_FIELD_REQUIRE(Int32, processedCount,
        ZH_WORDS_GETTER("inpatientbilling.field.processedCount"), true);
    API_DTO_FIELD_REQUIRE(List<String>, createdServiceRequestIds,
        ZH_WORDS_GETTER("inpatientbilling.field.createdServiceRequestIds"), false);

public:
    ConfirmDirectPricingResponseDTO()
    {
        success = true;
        processedCount = 1;
        createdServiceRequestIds = List<String>::createShared();
        createdServiceRequestIds->push_back("SR202607240001");
    }
};

// =======================================================================
// SECTION 3: List Priced Orders (GET /inpatientbilling/list-priced-orders)
// =======================================================================

class ListPricedOrdersQuery : public oatpp::DTO
{
    DTO_INIT(ListPricedOrdersQuery, DTO);

    API_DTO_FIELD_REQUIRE(List<String>, encounterIds,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterIds"), true);
    API_DTO_FIELD_REQUIRE(String, pricedTimeFrom,
        ZH_WORDS_GETTER("inpatientbilling.field.pricedTimeFrom"), false);
    API_DTO_FIELD_REQUIRE(String, pricedTimeTo,
        ZH_WORDS_GETTER("inpatientbilling.field.pricedTimeTo"), false);

public:
    ListPricedOrdersQuery()
    {
        encounterIds = List<String>::createShared();
        encounterIds->push_back("337907884548657152");
        pricedTimeFrom = "2026-07-24 00:00:00";
        pricedTimeTo = "2026-07-24 23:59:59";
    }
};

class PricedOrderItemDTO : public oatpp::DTO
{
    DTO_INIT(PricedOrderItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, serviceRequestId,
        ZH_WORDS_GETTER("inpatientbilling.field.serviceRequestId"), false);
    API_DTO_FIELD_REQUIRE(String, productDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.productDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(String, productName,
        ZH_WORDS_GETTER("inpatientbilling.field.productName"), false);
    API_DTO_FIELD_REQUIRE(Float64, quantity,
        ZH_WORDS_GETTER("inpatientbilling.field.quantity"), false);
    API_DTO_FIELD_REQUIRE(String, unitCode,
        ZH_WORDS_GETTER("inpatientbilling.field.unitCode"), false);
    API_DTO_FIELD_REQUIRE(Float64, unitPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.unitPrice"), false);
    API_DTO_FIELD_REQUIRE(Float64, totalPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.totalPrice"), false);
    API_DTO_FIELD_REQUIRE(String, pricedTime,
        ZH_WORDS_GETTER("inpatientbilling.field.pricedTime"), false);

public:
    PricedOrderItemDTO()
    {
        serviceRequestId = "SR202607240001";
        productDefinitionId = "334568157811871744";
        productName = "替米沙坦片";
        quantity = 2.0;
        unitCode = "片";
        unitPrice = 2.50;
        totalPrice = 5.00;
        pricedTime = "2026-07-24 10:00:00";
    }
};

class PricedOrderGroupDTO : public oatpp::DTO
{
    DTO_INIT(PricedOrderGroupDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(List<PricedOrderItemDTO::Wrapper>, pricedOrders,
        ZH_WORDS_GETTER("inpatientbilling.field.pricedOrders"), true);

public:
    PricedOrderGroupDTO()
    {
        encounterId = "337907884548657152";
        pricedOrders = List<PricedOrderItemDTO::Wrapper>::createShared();
        pricedOrders->push_back(PricedOrderItemDTO::createShared());
    }
};

class ListPricedOrdersDTO : public oatpp::DTO
{
    DTO_INIT(ListPricedOrdersDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<PricedOrderGroupDTO::Wrapper>, groups,
        ZH_WORDS_GETTER("inpatientbilling.field.groups"), true);

public:
    ListPricedOrdersDTO()
    {
        groups = List<PricedOrderGroupDTO::Wrapper>::createShared();
        groups->push_back(PricedOrderGroupDTO::createShared());
    }
};

// =======================================================================
// SECTION 4: List Supplement Parent Orders (GET /inpatientbilling/list-supplement-parent-orders)
// =======================================================================

class ListSupplementParentOrdersQuery : public oatpp::DTO
{
    DTO_INIT(ListSupplementParentOrdersQuery, DTO);

    API_DTO_FIELD_REQUIRE(List<String>, encounterIds,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterIds"), true);
    API_DTO_FIELD_REQUIRE(String, therapyType,
        ZH_WORDS_GETTER("inpatientbilling.field.therapyType"), false);
    API_DTO_FIELD_REQUIRE(String, keyword,
        ZH_WORDS_GETTER("inpatientbilling.field.keyword"), false);
    API_DTO_FIELD_REQUIRE(String, orgId,
        ZH_WORDS_GETTER("inpatientbilling.field.orgId"), false);

public:
    ListSupplementParentOrdersQuery()
    {
        encounterIds = List<String>::createShared();
        encounterIds->push_back("337907884548657152");
        therapyType = "TEMPORARY";
        keyword = "替米沙坦";
        orgId = "327170091059744768";
    }
};

class SupplementParentOrderItemDTO : public oatpp::DTO
{
    DTO_INIT(SupplementParentOrderItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, orderId,
        ZH_WORDS_GETTER("inpatientbilling.field.orderId"), false);
    API_DTO_FIELD_REQUIRE(String, orderName,
        ZH_WORDS_GETTER("inpatientbilling.field.orderName"), false);
    API_DTO_FIELD_REQUIRE(String, therapyType,
        ZH_WORDS_GETTER("inpatientbilling.field.therapyType"), false);
    API_DTO_FIELD_REQUIRE(String, therapyTypeText,
        ZH_WORDS_GETTER("inpatientbilling.field.therapyTypeText"), false);
    API_DTO_FIELD_REQUIRE(String, dosageInstruction,
        ZH_WORDS_GETTER("inpatientbilling.field.dosageInstruction"), false);
    API_DTO_FIELD_REQUIRE(String, frequencyCode,
        ZH_WORDS_GETTER("inpatientbilling.field.frequencyCode"), false);
    API_DTO_FIELD_REQUIRE(String, frequencyCodeText,
        ZH_WORDS_GETTER("inpatientbilling.field.frequencyCodeText"), false);

public:
    SupplementParentOrderItemDTO()
    {
        orderId = "337990013773271040";
        orderName = "替米沙坦片";
        therapyType = "TEMPORARY";
        therapyTypeText = "临时";
        dosageInstruction = "40mg";
        frequencyCode = "QD";
        frequencyCodeText = "每日一次";
    }
};

class SupplementParentOrderGroupDTO : public oatpp::DTO
{
    DTO_INIT(SupplementParentOrderGroupDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(List<SupplementParentOrderItemDTO::Wrapper>, orders,
        ZH_WORDS_GETTER("inpatientbilling.field.orders"), true);

public:
    SupplementParentOrderGroupDTO()
    {
        encounterId = "337907884548657152";
        orders = List<SupplementParentOrderItemDTO::Wrapper>::createShared();
        orders->push_back(SupplementParentOrderItemDTO::createShared());
    }
};

class ListSupplementParentOrdersDTO : public oatpp::DTO
{
    DTO_INIT(ListSupplementParentOrdersDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<SupplementParentOrderGroupDTO::Wrapper>, groups,
        ZH_WORDS_GETTER("inpatientbilling.field.groups"), true);

public:
    ListSupplementParentOrdersDTO()
    {
        groups = List<SupplementParentOrderGroupDTO::Wrapper>::createShared();
        groups->push_back(SupplementParentOrderGroupDTO::createShared());
    }
};

// =======================================================================
// SECTION 5: Confirm Supplement Pricing (POST /inpatientbilling/confirm-supplement-pricing)
// =======================================================================

class ConfirmSupplementPricingItemDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmSupplementPricingItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(String, accountId,
        ZH_WORDS_GETTER("inpatientbilling.field.accountId"), true);
    API_DTO_FIELD_REQUIRE(String, productTableName,
        ZH_WORDS_GETTER("inpatientbilling.field.productTableName"), true);
    API_DTO_FIELD_REQUIRE(String, productDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.productDefinitionId"), true);
    API_DTO_FIELD_REQUIRE(Float64, quantity,
        ZH_WORDS_GETTER("inpatientbilling.field.quantity"), true);
    API_DTO_FIELD_REQUIRE(String, unitCode,
        ZH_WORDS_GETTER("inpatientbilling.field.unitCode"), true);
    API_DTO_FIELD_REQUIRE(String, chargeItemDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.chargeItemDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(Float64, unitPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.unitPrice"), true);
    API_DTO_FIELD_REQUIRE(Float64, totalPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.totalPrice"), true);
    API_DTO_FIELD_REQUIRE(String, remark,
        ZH_WORDS_GETTER("inpatientbilling.field.remark"), false);
    API_DTO_FIELD_REQUIRE(String, parentRequestAnchor,
        ZH_WORDS_GETTER("inpatientbilling.field.parentRequestAnchor"), true);

public:
    ConfirmSupplementPricingItemDTO()
    {
        encounterId = "337907884548657152";
        patientId = "326940849401761792";
        accountId = "337907884548657153";
        productTableName = "product_table";
        productDefinitionId = "334568157811871744";
        quantity = 2.0;
        unitCode = "片";
        chargeItemDefinitionId = "334568157811871745";
        unitPrice = 2.50;
        totalPrice = 5.00;
        remark = "Supplement charge item";
        parentRequestAnchor = "ORDER_337990013773271040";
    }
};

class ConfirmSupplementPricingDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmSupplementPricingDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<ConfirmSupplementPricingItemDTO::Wrapper>, items,
        ZH_WORDS_GETTER("inpatientbilling.field.items"), true);

public:
    ConfirmSupplementPricingDTO()
    {
        items = List<ConfirmSupplementPricingItemDTO::Wrapper>::createShared();
        items->push_back(ConfirmSupplementPricingItemDTO::createShared());
    }
};

class ConfirmSupplementPricingResponseDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmSupplementPricingResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);
    API_DTO_FIELD_REQUIRE(Int32, processedCount,
        ZH_WORDS_GETTER("inpatientbilling.field.processedCount"), true);
    API_DTO_FIELD_REQUIRE(List<String>, createdServiceRequestIds,
        ZH_WORDS_GETTER("inpatientbilling.field.createdServiceRequestIds"), false);

public:
    ConfirmSupplementPricingResponseDTO()
    {
        success = true;
        processedCount = 1;
        createdServiceRequestIds = List<String>::createShared();
        createdServiceRequestIds->push_back("SR202607240002");
    }
};

// =======================================================================
// SECTION 6: Cancel Pricing (POST /inpatientbilling/cancel-pricing)
// =======================================================================

class CancelPricingItemDTO : public oatpp::DTO
{
    DTO_INIT(CancelPricingItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, orderType,
        ZH_WORDS_GETTER("inpatientbilling.field.orderType"), true);
    API_DTO_FIELD_REQUIRE(String, requestId,
        ZH_WORDS_GETTER("inpatientbilling.field.requestId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);

public:
    CancelPricingItemDTO()
    {
        orderType = "DIRECT";
        requestId = "SR202607240001";
        encounterId = "337907884548657152";
    }
};

class CancelPricingDTO : public oatpp::DTO
{
    DTO_INIT(CancelPricingDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<CancelPricingItemDTO::Wrapper>, items,
        ZH_WORDS_GETTER("inpatientbilling.field.items"), true);

public:
    CancelPricingDTO()
    {
        items = List<CancelPricingItemDTO::Wrapper>::createShared();
        items->push_back(CancelPricingItemDTO::createShared());
    }
};

class CancelPricingResponseDTO : public oatpp::DTO
{
    DTO_INIT(CancelPricingResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);
    API_DTO_FIELD_REQUIRE(Int32, processedCount,
        ZH_WORDS_GETTER("inpatientbilling.field.processedCount"), true);
    API_DTO_FIELD_REQUIRE(List<String>, cancelledServiceRequestIds,
        ZH_WORDS_GETTER("inpatientbilling.field.cancelledServiceRequestIds"), false);

public:
    CancelPricingResponseDTO()
    {
        success = true;
        processedCount = 1;
        cancelledServiceRequestIds = List<String>::createShared();
        cancelledServiceRequestIds->push_back("SR202607240001");
    }
};

// =======================================================================
// SECTION 7: List Auto Roll Configs (GET /inpatientbilling/list-auto-roll-configs)
// =======================================================================

class ListAutoRollConfigsQuery : public oatpp::DTO
{
    DTO_INIT(ListAutoRollConfigsQuery, DTO);

    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(String, billDate,
        ZH_WORDS_GETTER("inpatientbilling.field.billDate"), false);

public:
    ListAutoRollConfigsQuery()
    {
        encounterId = "337907884548657152";
        patientId = "326940849401761792";
        billDate = "2026-07-24";
    }
};

class AutoRollConfigDTO : public oatpp::DTO
{
    DTO_INIT(AutoRollConfigDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), false);
    API_DTO_FIELD_REQUIRE(String, ruleName,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleName"), false);
    API_DTO_FIELD_REQUIRE(String, activityId,
        ZH_WORDS_GETTER("inpatientbilling.field.activityId"), false);
    API_DTO_FIELD_REQUIRE(String, activityName,
        ZH_WORDS_GETTER("inpatientbilling.field.activityName"), false);
    API_DTO_FIELD_REQUIRE(String, chargeDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.chargeDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(Float64, quantityValue,
        ZH_WORDS_GETTER("inpatientbilling.field.quantityValue"), false);
    API_DTO_FIELD_REQUIRE(Boolean, isActive,
        ZH_WORDS_GETTER("inpatientbilling.field.isActive"), false);

public:
    AutoRollConfigDTO()
    {
        ruleId = "RUL202607240001";
        ruleName = "早晨常规计费";
        activityId = "ACT_MORNING_ROUTINE";
        activityName = "晨间护理";
        chargeDefinitionId = "CDEF_NURSING_CARE";
        quantityValue = 1.0;
        isActive = true;
    }
};

class ListAutoRollConfigsDTO : public oatpp::DTO
{
    DTO_INIT(ListAutoRollConfigsDTO, DTO);

    API_DTO_FIELD_REQUIRE(List<AutoRollConfigDTO::Wrapper>, configs,
        ZH_WORDS_GETTER("inpatientbilling.field.configs"), true);
    API_DTO_FIELD_REQUIRE(Boolean, nursingLevelReadonly,
        ZH_WORDS_GETTER("inpatientbilling.field.nursingLevelReadonly"), true);

public:
    ListAutoRollConfigsDTO()
    {
        configs = List<AutoRollConfigDTO::Wrapper>::createShared();
        configs->push_back(AutoRollConfigDTO::createShared());
        nursingLevelReadonly = false;
    }
};

// =======================================================================
// SECTION 8: Create Auto Gen Rule (POST /inpatientbilling/create-auto-gen-rule)
// =======================================================================

class CreateAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(CreateAutoGenRuleDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(String, ruleName,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleName"), true);
    API_DTO_FIELD_REQUIRE(String, activityId,
        ZH_WORDS_GETTER("inpatientbilling.field.activityId"), true);
    API_DTO_FIELD_REQUIRE(String, chargeDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.chargeDefinitionId"), true);
    API_DTO_FIELD_REQUIRE(Float64, quantityValue,
        ZH_WORDS_GETTER("inpatientbilling.field.quantityValue"), false);
    API_DTO_FIELD_REQUIRE(String, performingOrgId,
        ZH_WORDS_GETTER("inpatientbilling.field.performingOrgId"), false);
    API_DTO_FIELD_REQUIRE(String, priority,
        ZH_WORDS_GETTER("inpatientbilling.field.priority"), false);
    API_DTO_FIELD_REQUIRE(String, remark,
        ZH_WORDS_GETTER("inpatientbilling.field.remark"), false);

public:
    CreateAutoGenRuleDTO()
    {
        encounterId = "337907884548657152";
        patientId = "326940849401761792";
        ruleName = "早晨常规计费规则";
        activityId = "ACT_MORNING_ROUTINE";
        chargeDefinitionId = "CDEF_NURSING_CARE";
        quantityValue = 1.0;
        performingOrgId = "327170091059744768";
        priority = "NORMAL";
        remark = "Auto-generated billing rule";
    }
};

class CreateAutoGenRuleResponseDTO : public oatpp::DTO
{
    DTO_INIT(CreateAutoGenRuleResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), true);

public:
    CreateAutoGenRuleResponseDTO()
    {
        ruleId = "RUL202607240001";
    }
};

// =======================================================================
// SECTION 9: Update Auto Gen Rule (POST /inpatientbilling/update-auto-gen-rule)
// =======================================================================

class UpdateAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(UpdateAutoGenRuleDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(String, ruleName,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleName"), false);
    API_DTO_FIELD_REQUIRE(String, activityId,
        ZH_WORDS_GETTER("inpatientbilling.field.activityId"), false);
    API_DTO_FIELD_REQUIRE(String, chargeDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.chargeDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(Float64, quantityValue,
        ZH_WORDS_GETTER("inpatientbilling.field.quantityValue"), false);
    API_DTO_FIELD_REQUIRE(String, performingOrgId,
        ZH_WORDS_GETTER("inpatientbilling.field.performingOrgId"), false);
    API_DTO_FIELD_REQUIRE(String, priority,
        ZH_WORDS_GETTER("inpatientbilling.field.priority"), false);
    API_DTO_FIELD_REQUIRE(String, remark,
        ZH_WORDS_GETTER("inpatientbilling.field.remark"), false);

public:
    UpdateAutoGenRuleDTO()
    {
        ruleId = "RUL202607240001";
        encounterId = "337907884548657152";
        patientId = "326940849401761792";
        ruleName = "更新早晨常规计费规则";
        activityId = "ACT_MORNING_ROUTINE";
        chargeDefinitionId = "CDEF_NURSING_CARE";
        quantityValue = 2.0;
        performingOrgId = "327170091059744768";
        priority = "NORMAL";
        remark = "Updated billing rule";
    }
};

class UpdateAutoGenRuleResponseDTO : public oatpp::DTO
{
    DTO_INIT(UpdateAutoGenRuleResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);

public:
    UpdateAutoGenRuleResponseDTO()
    {
        success = true;
    }
};

// =======================================================================
// SECTION 10: Delete Auto Gen Rule (POST /inpatientbilling/delete-auto-gen-rule)
// =======================================================================

class DeleteAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(DeleteAutoGenRuleDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);

public:
    DeleteAutoGenRuleDTO()
    {
        ruleId = "RUL202607240001";
        encounterId = "337907884548657152";
        patientId = "326940849401761792";
    }
};

class DeleteAutoGenRuleResponseDTO : public oatpp::DTO
{
    DTO_INIT(DeleteAutoGenRuleResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);

public:
    DeleteAutoGenRuleResponseDTO()
    {
        success = true;
    }
};

// =======================================================================
// SECTION 11: Activate/Suspend Auto Gen Rule (POST /inpatientbilling/activate-auto-gen-rule and suspend)
// =======================================================================

class ActivateAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(ActivateAutoGenRuleDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);

public:
    ActivateAutoGenRuleDTO()
    {
        ruleId = "RUL202607240001";
        encounterId = "337907884548657152";
        patientId = "326940849401761792";
    }
};

class ActivateAutoGenRuleResponseDTO : public oatpp::DTO
{
    DTO_INIT(ActivateAutoGenRuleResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);

public:
    ActivateAutoGenRuleResponseDTO()
    {
        success = true;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INPATIENT_BILLING_DTO_H_
