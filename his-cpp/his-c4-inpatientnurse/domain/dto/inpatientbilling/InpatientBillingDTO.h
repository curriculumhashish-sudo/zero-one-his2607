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
    DTO_INIT(PagePricingCatalogQuery, PageQuery);

	    API_DTO_FIELD_REQUIRE(String, encounterId,
	        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
	    API_DTO_FIELD_REQUIRE(String, patientId,
	        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);
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
    API_DTO_FIELD_REQUIRE(String, productTableName,
        ZH_WORDS_GETTER("inpatientbilling.field.productTableName"), false);
    API_DTO_FIELD_REQUIRE(String, chargeItemDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.chargeItemDefinitionId"), false);
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
    API_DTO_FIELD_REQUIRE(String, defaultDispenseLocationId,
        ZH_WORDS_GETTER("inpatientbilling.field.defaultDispenseLocationId"), false);

public:
    NurseStationPricingCatalogRowDTO()
    {
        unitPrice = 0.0;
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
        this->total = static_cast<v_int64>(0);
        this->pages = static_cast<v_int64>(0);
        this->rows = List<NurseStationPricingCatalogRowDTO::Wrapper>::createShared();
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
        ZH_WORDS_GETTER("inpatientbilling.field.enabled"), false);
    API_DTO_FIELD_REQUIRE(String, accountId,
        ZH_WORDS_GETTER("inpatientbilling.field.accountId"), true);
    API_DTO_FIELD_REQUIRE(String, productTableName,
        ZH_WORDS_GETTER("inpatientbilling.field.productTableName"), false);
    API_DTO_FIELD_REQUIRE(String, productDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.productDefinitionId"), true);
    API_DTO_FIELD_REQUIRE(String, chargeItemDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.chargeItemDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(Float64, quantity,
        ZH_WORDS_GETTER("inpatientbilling.field.quantity"), true);
    API_DTO_FIELD_REQUIRE(String, unitCode,
        ZH_WORDS_GETTER("inpatientbilling.field.unitCode"), true);
    API_DTO_FIELD_REQUIRE(Float64, unitPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.unitPrice"), false);
    API_DTO_FIELD_REQUIRE(Float64, totalPrice,
        ZH_WORDS_GETTER("inpatientbilling.field.totalPrice"), false);
    API_DTO_FIELD_REQUIRE(String, remark,
        ZH_WORDS_GETTER("inpatientbilling.field.remark"), false);
    API_DTO_FIELD_REQUIRE(String, performLocation,
        ZH_WORDS_GETTER("inpatientbilling.field.performLocation"), false);
	API_DTO_FIELD_REQUIRE(String, clientRowId,
		ZH_WORDS_GETTER("inpatientbilling.field.clientRowId"), false);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->encounterId = "337907884548657152";
        dto->patientId = "326940849401761792";
        dto->enabled = true;
        dto->accountId = "337907884548657153";
        dto->productTableName = "wkf_activity_definition";
        dto->productDefinitionId = "334568157811871744";
        dto->quantity = 2.0;
        dto->unitCode = ZH_WORDS_GETTER("business-text.sample.tablet-unit");
        dto->chargeItemDefinitionId = "334568157811871745";
        dto->unitPrice = 2.50;
        dto->totalPrice = 5.00;
        dto->remark = "Direct charge item";
        dto->performLocation = "";
		dto->clientRowId = "DIRECT-ROW-001";
        return dto;
    }
};

class ConfirmDirectPricingDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmDirectPricingDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("inpatientbilling.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(List<ConfirmDirectPricingItemDTO::Wrapper>, items,
        ZH_WORDS_GETTER("inpatientbilling.field.items"), true);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
		dto->idempotencyKey = "DIRECT-PRICING-20260813-0001";
        dto->items = List<ConfirmDirectPricingItemDTO::Wrapper>::createShared();
        dto->items->push_back(ConfirmDirectPricingItemDTO::createExample());
        return dto;
    }
};

class PricingWriteResultDTO : public oatpp::DTO
{
	DTO_INIT(PricingWriteResultDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, clientRowId,
		ZH_WORDS_GETTER("inpatientbilling.field.clientRowId"), false);
	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("inpatientbilling.field.success"), true);
	API_DTO_FIELD_REQUIRE(String, requestId,
		ZH_WORDS_GETTER("inpatientbilling.field.requestId"), false);
	API_DTO_FIELD_REQUIRE(Float64, finalUnitPrice,
		ZH_WORDS_GETTER("inpatientbilling.field.finalUnitPrice"), false);
	API_DTO_FIELD_REQUIRE(Float64, finalTotalPrice,
		ZH_WORDS_GETTER("inpatientbilling.field.finalTotalPrice"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("inpatientbilling.field.message"), false);

public:
	PricingWriteResultDTO()
	{
		success = false;
		finalUnitPrice = 0.0;
		finalTotalPrice = 0.0;
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
    API_DTO_FIELD_REQUIRE(List<String>, createdDeviceRequestIds,
        ZH_WORDS_GETTER("inpatientbilling.field.createdDeviceRequestIds"), false);
	API_DTO_FIELD_REQUIRE(List<PricingWriteResultDTO::Wrapper>, results,
		ZH_WORDS_GETTER("inpatientbilling.field.results"), true);

public:
    ConfirmDirectPricingResponseDTO()
    {
        success = false;
        processedCount = 0;
        createdServiceRequestIds = List<String>::createShared();
        createdDeviceRequestIds = List<String>::createShared();
		results = List<PricingWriteResultDTO::Wrapper>::createShared();
    }
};

// =======================================================================
// SECTION 3: List Priced Orders (GET /inpatientbilling/list-priced-orders)
// =======================================================================

class ListPricedOrdersQuery : public oatpp::DTO
{
    DTO_INIT(ListPricedOrdersQuery, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, encounterIds,
		ZH_WORDS_GETTER("inpatientbilling.field.encounterIds"), false);
    API_DTO_FIELD_REQUIRE(String, pricedTimeFrom,
        ZH_WORDS_GETTER("inpatientbilling.field.pricedTimeFrom"), false);
    API_DTO_FIELD_REQUIRE(String, pricedTimeTo,
        ZH_WORDS_GETTER("inpatientbilling.field.pricedTimeTo"), false);

public:
    ListPricedOrdersQuery() = default;
};

class PricedOrderItemDTO : public oatpp::DTO
{
    DTO_INIT(PricedOrderItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, serviceRequestId,
        ZH_WORDS_GETTER("inpatientbilling.field.serviceRequestId"), false);
	API_DTO_FIELD_REQUIRE(String, requestId,
		ZH_WORDS_GETTER("inpatientbilling.field.requestId"), false);
    API_DTO_FIELD_REQUIRE(String, orderType,
        ZH_WORDS_GETTER("inpatientbilling.field.orderType"), false);
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
	API_DTO_FIELD_REQUIRE(String, pricingMode,
		ZH_WORDS_GETTER("inpatientbilling.field.pricingMode"), false);
	API_DTO_FIELD_REQUIRE(String, parentRequestId,
		ZH_WORDS_GETTER("inpatientbilling.field.parentRequestId"), false);
	API_DTO_FIELD_REQUIRE(String, chargeStatusEnum,
		ZH_WORDS_GETTER("inpatientbilling.field.chargeStatusEnum"), false);
	API_DTO_FIELD_REQUIRE(String, dispenseStatusEnum,
		ZH_WORDS_GETTER("inpatientbilling.field.dispenseStatusEnum"), false);

public:
    PricedOrderItemDTO()
    {
        quantity = 0.0;
        unitPrice = 0.0;
        totalPrice = 0.0;
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
        pricedOrders = List<PricedOrderItemDTO::Wrapper>::createShared();
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
    }
};

// =======================================================================
// SECTION 4: List Supplement Parent Orders (GET /inpatientbilling/list-supplement-parent-orders)
// =======================================================================

class ListSupplementParentOrdersQuery : public oatpp::DTO
{
    DTO_INIT(ListSupplementParentOrdersQuery, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(String, encounterIds,
		ZH_WORDS_GETTER("inpatientbilling.field.encounterIds"), false);
    API_DTO_FIELD_REQUIRE(String, therapyType,
        ZH_WORDS_GETTER("inpatientbilling.field.therapyType"), false);
    API_DTO_FIELD_REQUIRE(String, keyword,
        ZH_WORDS_GETTER("inpatientbilling.field.keyword"), false);
    API_DTO_FIELD_REQUIRE(String, orgId,
        ZH_WORDS_GETTER("inpatientbilling.field.orgId"), false);

public:
    ListSupplementParentOrdersQuery() = default;
};

class SupplementParentOrderItemDTO : public oatpp::DTO
{
    DTO_INIT(SupplementParentOrderItemDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, orderId,
        ZH_WORDS_GETTER("inpatientbilling.field.orderId"), false);
	API_DTO_FIELD_REQUIRE(String, requestId,
		ZH_WORDS_GETTER("inpatientbilling.field.requestId"), false);
	API_DTO_FIELD_REQUIRE(String, orderType,
		ZH_WORDS_GETTER("inpatientbilling.field.orderType"), false);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("inpatientbilling.field.statusEnum"), false);
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
	API_DTO_FIELD_REQUIRE(String, specificationText,
		ZH_WORDS_GETTER("inpatientbilling.field.specificationText"), false);
	API_DTO_FIELD_REQUIRE(String, authoredTime,
		ZH_WORDS_GETTER("inpatientbilling.field.authoredTime"), false);
	API_DTO_FIELD_REQUIRE(String, stopTime,
		ZH_WORDS_GETTER("inpatientbilling.field.stopTime"), false);
	API_DTO_FIELD_REQUIRE(String, requesterName,
		ZH_WORDS_GETTER("inpatientbilling.field.requesterName"), false);
	API_DTO_FIELD_REQUIRE(String, performingLocationName,
		ZH_WORDS_GETTER("inpatientbilling.field.performingLocationName"), false);

public:
    SupplementParentOrderItemDTO()
    {
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
        orders = List<SupplementParentOrderItemDTO::Wrapper>::createShared();
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
		ZH_WORDS_GETTER("inpatientbilling.field.productTableName"), false);
    API_DTO_FIELD_REQUIRE(String, productDefinitionId,
        ZH_WORDS_GETTER("inpatientbilling.field.productDefinitionId"), true);
    API_DTO_FIELD_REQUIRE(Float64, quantity,
        ZH_WORDS_GETTER("inpatientbilling.field.quantity"), true);
    API_DTO_FIELD_REQUIRE(String, unitCode,
        ZH_WORDS_GETTER("inpatientbilling.field.unitCode"), true);
    API_DTO_FIELD_REQUIRE(String, chargeItemDefinitionId,
		ZH_WORDS_GETTER("inpatientbilling.field.chargeItemDefinitionId"), false);
    API_DTO_FIELD_REQUIRE(Float64, unitPrice,
		ZH_WORDS_GETTER("inpatientbilling.field.unitPrice"), false);
    API_DTO_FIELD_REQUIRE(Float64, totalPrice,
		ZH_WORDS_GETTER("inpatientbilling.field.totalPrice"), false);
    API_DTO_FIELD_REQUIRE(String, remark,
        ZH_WORDS_GETTER("inpatientbilling.field.remark"), false);
    API_DTO_FIELD_REQUIRE(String, parentRequestAnchor,
		ZH_WORDS_GETTER("inpatientbilling.field.parentRequestAnchor"), false);
	API_DTO_FIELD_REQUIRE(String, parentOrderType,
		ZH_WORDS_GETTER("inpatientbilling.field.parentOrderType"), true);
	API_DTO_FIELD_REQUIRE(String, parentRequestId,
		ZH_WORDS_GETTER("inpatientbilling.field.parentRequestId"), true);
    API_DTO_FIELD_REQUIRE(String, performLocation,
        ZH_WORDS_GETTER("inpatientbilling.field.performLocation"), false);
	API_DTO_FIELD_REQUIRE(String, clientRowId,
		ZH_WORDS_GETTER("inpatientbilling.field.clientRowId"), false);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->encounterId = "337907884548657152";
        dto->patientId = "326940849401761792";
        dto->accountId = "337907884548657153";
        dto->productTableName = "wkf_activity_definition";
        dto->productDefinitionId = "334568157811871744";
        dto->quantity = 2.0;
        dto->unitCode = ZH_WORDS_GETTER("business-text.sample.tablet-unit");
        dto->chargeItemDefinitionId = "334568157811871745";
        dto->unitPrice = 2.50;
        dto->totalPrice = 5.00;
        dto->remark = "Supplement charge item";
        dto->parentRequestAnchor = "ORDER_337990013773271040";
		dto->parentOrderType = "SERVICE_REQUEST";
		dto->parentRequestId = "337990013773271040";
        dto->performLocation = "";
		dto->clientRowId = "SUPPLEMENT-ROW-001";
        return dto;
    }
};

class ConfirmSupplementPricingDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmSupplementPricingDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("inpatientbilling.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(List<ConfirmSupplementPricingItemDTO::Wrapper>, items,
        ZH_WORDS_GETTER("inpatientbilling.field.items"), true);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
		dto->idempotencyKey = "SUPPLEMENT-PRICING-20260813-0001";
        dto->items = List<ConfirmSupplementPricingItemDTO::Wrapper>::createShared();
        dto->items->push_back(ConfirmSupplementPricingItemDTO::createExample());
        return dto;
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
    API_DTO_FIELD_REQUIRE(List<String>, createdDeviceRequestIds,
        ZH_WORDS_GETTER("inpatientbilling.field.createdDeviceRequestIds"), false);
	API_DTO_FIELD_REQUIRE(List<PricingWriteResultDTO::Wrapper>, results,
		ZH_WORDS_GETTER("inpatientbilling.field.results"), true);

public:
    ConfirmSupplementPricingResponseDTO()
    {
        success = false;
        processedCount = 0;
        createdServiceRequestIds = List<String>::createShared();
        createdDeviceRequestIds = List<String>::createShared();
		results = List<PricingWriteResultDTO::Wrapper>::createShared();
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
    static Wrapper createExample()
    {
        auto dto = createShared();
        dto->orderType = "DIRECT";
        dto->requestId = "SR202607240001";
        dto->encounterId = "337907884548657152";
        return dto;
    }
};

class CancelPricingDTO : public oatpp::DTO
{
    DTO_INIT(CancelPricingDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("inpatientbilling.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(String, reason,
		ZH_WORDS_GETTER("inpatientbilling.field.reason"), true);
	API_DTO_FIELD_REQUIRE(List<CancelPricingItemDTO::Wrapper>, items,
        ZH_WORDS_GETTER("inpatientbilling.field.items"), true);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
		dto->idempotencyKey = "CANCEL-PRICING-20260813-0001";
		dto->reason = ZH_WORDS_GETTER("business-text.billing.cancel-reason");
        dto->items = List<CancelPricingItemDTO::Wrapper>::createShared();
        dto->items->push_back(CancelPricingItemDTO::createExample());
        return dto;
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
    API_DTO_FIELD_REQUIRE(List<String>, cancelledDeviceRequestIds,
        ZH_WORDS_GETTER("inpatientbilling.field.cancelledDeviceRequestIds"), false);
	API_DTO_FIELD_REQUIRE(List<PricingWriteResultDTO::Wrapper>, results,
		ZH_WORDS_GETTER("inpatientbilling.field.results"), true);

public:
    CancelPricingResponseDTO()
    {
        success = false;
        processedCount = 0;
        cancelledServiceRequestIds = List<String>::createShared();
        cancelledDeviceRequestIds = List<String>::createShared();
		results = List<PricingWriteResultDTO::Wrapper>::createShared();
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
    ListAutoRollConfigsQuery() = default;
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
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("inpatientbilling.field.version"), true);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("inpatientbilling.field.statusEnum"), true);
	API_DTO_FIELD_REQUIRE(String, triggerSourceTable,
		ZH_WORDS_GETTER("inpatientbilling.field.triggerSourceTable"), false);
	API_DTO_FIELD_REQUIRE(String, performingOrgId,
		ZH_WORDS_GETTER("inpatientbilling.field.performingOrgId"), false);
	API_DTO_FIELD_REQUIRE(Int32, priority,
		ZH_WORDS_GETTER("inpatientbilling.field.priority"), false);
	API_DTO_FIELD_REQUIRE(String, remark,
		ZH_WORDS_GETTER("inpatientbilling.field.remark"), false);
	API_DTO_FIELD_REQUIRE(String, updatedAt,
		ZH_WORDS_GETTER("inpatientbilling.field.updatedAt"), false);

public:
    AutoRollConfigDTO()
    {
        quantityValue = 0.0;
        isActive = false;
		version = static_cast<v_int64>(0);
		priority = 0;
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
        nursingLevelReadonly = false;
    }
};

// =======================================================================
// SECTION 8: Create Auto Gen Rule (POST /inpatientbilling/create-auto-gen-rule)
// =======================================================================

class CreateAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(CreateAutoGenRuleDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("inpatientbilling.field.idempotencyKey"), true);
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
    static Wrapper createExample()
    {
        auto dto = createShared();
		dto->idempotencyKey = "AUTO-RULE-CREATE-20260813-0001";
        dto->encounterId = "337907884548657152";
        dto->patientId = "326940849401761792";
        dto->ruleName = ZH_WORDS_GETTER("business-text.sample.billing-rule-create-name");
        dto->activityId = "ACT_MORNING_ROUTINE";
        dto->chargeDefinitionId = "CDEF_NURSING_CARE";
        dto->quantityValue = 1.0;
        dto->performingOrgId = "327170091059744768";
        dto->priority = "NORMAL";
        dto->remark = "Auto-generated billing rule";
        return dto;
    }
};

class CreateAutoGenRuleResponseDTO : public oatpp::DTO
{
    DTO_INIT(CreateAutoGenRuleResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), true);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("inpatientbilling.field.version"), true);

public:
    CreateAutoGenRuleResponseDTO()
    {
		version = static_cast<v_int64>(0);
    }
};

// =======================================================================
// SECTION 9: Update Auto Gen Rule (POST /inpatientbilling/update-auto-gen-rule)
// =======================================================================

class UpdateAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(UpdateAutoGenRuleDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("inpatientbilling.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("inpatientbilling.field.expectedVersion"), true);
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
	UpdateAutoGenRuleDTO() = default;
};

class UpdateAutoGenRuleResponseDTO : public oatpp::DTO
{
    DTO_INIT(UpdateAutoGenRuleResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("inpatientbilling.field.version"), false);

public:
    UpdateAutoGenRuleResponseDTO()
    {
        success = false;
    }
};

// =======================================================================
// SECTION 10: Delete Auto Gen Rule (POST /inpatientbilling/delete-auto-gen-rule)
// =======================================================================

class DeleteAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(DeleteAutoGenRuleDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("inpatientbilling.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("inpatientbilling.field.expectedVersion"), true);
	API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
		dto->idempotencyKey = "AUTO-RULE-DELETE-20260813-0001";
		dto->expectedVersion = static_cast<v_int64>(0);
        dto->ruleId = "RUL202607240001";
        dto->encounterId = "337907884548657152";
        dto->patientId = "326940849401761792";
        return dto;
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
        success = false;
    }
};

// =======================================================================
// SECTION 11: Activate/Suspend Auto Gen Rule (POST /inpatientbilling/activate-auto-gen-rule and suspend)
// =======================================================================

class ActivateAutoGenRuleDTO : public oatpp::DTO
{
    DTO_INIT(ActivateAutoGenRuleDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("inpatientbilling.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(Int64, expectedVersion,
		ZH_WORDS_GETTER("inpatientbilling.field.expectedVersion"), true);
	API_DTO_FIELD_REQUIRE(String, ruleId,
        ZH_WORDS_GETTER("inpatientbilling.field.ruleId"), true);
    API_DTO_FIELD_REQUIRE(String, encounterId,
        ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), true);
    API_DTO_FIELD_REQUIRE(String, patientId,
        ZH_WORDS_GETTER("inpatientbilling.field.patientId"), true);
    API_DTO_FIELD_REQUIRE(Boolean, enabled,
		ZH_WORDS_GETTER("inpatientbilling.field.enabled"), false);
	API_DTO_FIELD_REQUIRE(String, targetStatus,
		ZH_WORDS_GETTER("inpatientbilling.field.targetStatus"), true);

public:
    static Wrapper createExample()
    {
        auto dto = createShared();
		dto->idempotencyKey = "AUTO-RULE-STATUS-20260813-0001";
		dto->expectedVersion = static_cast<v_int64>(0);
        dto->ruleId = "RUL202607240001";
        dto->encounterId = "337907884548657152";
        dto->patientId = "326940849401761792";
        dto->enabled = true;
		dto->targetStatus = "ACTIVE";
        return dto;
    }
};

class ActivateAutoGenRuleResponseDTO : public oatpp::DTO
{
    DTO_INIT(ActivateAutoGenRuleResponseDTO, DTO);

    API_DTO_FIELD_REQUIRE(Boolean, success,
        ZH_WORDS_GETTER("inpatientbilling.field.success"), true);
	API_DTO_FIELD_REQUIRE(Int64, version,
		ZH_WORDS_GETTER("inpatientbilling.field.version"), false);
	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("inpatientbilling.field.statusEnum"), false);

public:
    ActivateAutoGenRuleResponseDTO()
    {
        success = false;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INPATIENT_BILLING_DTO_H_
