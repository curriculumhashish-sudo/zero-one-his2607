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
#ifndef _INPATIENT_BILLING_CONTROLLER_
#define _INPATIENT_BILLING_CONTROLLER_

#include "domain/dto/inpatientbilling/InpatientBillingDTO.h"
#include "domain/vo/inpatientbilling/InpatientBillingVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("inpatientbilling.tag")

class InpatientBillingController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(InpatientBillingController);

public:
    // 1. Page pricing catalog (GET)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.page-pricing-catalog.summary"),
        pagePricingCatalog,
        PagePricingCatalogJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_PAGE_PARAMS();
        info->queryParams["pageIndex"].required = true;
        info->queryParams["pageSize"].required = true;
        API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
            ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), "337907884548657152", false);
        API_DEF_ADD_QUERY_PARAMS(String, "patientId",
            ZH_WORDS_GETTER("inpatientbilling.field.patientId"), "326940849401761792", false);
        API_DEF_ADD_QUERY_PARAMS(String, "keyword",
            ZH_WORDS_GETTER("inpatientbilling.field.keyword"), ZH_WORDS_GETTER("business-text.sample.medication-keyword"), false);
        API_DEF_ADD_QUERY_PARAMS(String, "categoryCode",
            ZH_WORDS_GETTER("inpatientbilling.field.categoryCode"), "DRUG", false);
        API_DEF_ADD_QUERY_PARAMS(String, "organizationUnitId",
            ZH_WORDS_GETTER("inpatientbilling.field.organizationUnitId"), "334526752078102528", false);
        info->responses[Status::CODE_200].addExample(
            "success", PagePricingCatalogJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/inpatientbilling/page-pricing-catalog",
        pagePricingCatalog,
        PagePricingCatalogQuery,
        execPagePricingCatalog(query));

    // 2. Confirm direct pricing (POST)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.confirm-direct-pricing.summary"),
        confirmDirectPricing,
        ConfirmDirectPricingJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", ConfirmDirectPricingDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", ConfirmDirectPricingJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/inpatientbilling/confirm-direct-pricing",
        confirmDirectPricing,
        BODY_DTO(ConfirmDirectPricingDTO::Wrapper, dto),
        execConfirmDirectPricing(dto));

    // 3. List priced orders (GET)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.list-priced-orders.summary"),
        listPricedOrders,
        ListPricedOrdersJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "encounterIds",
            ZH_WORDS_GETTER("inpatientbilling.field.encounterIds"),
            "337907884548657152,337907884548657153", true);
        API_DEF_ADD_QUERY_PARAMS(String, "pricedTimeFrom",
            ZH_WORDS_GETTER("inpatientbilling.field.pricedTimeFrom"), "2026-07-24 00:00:00", false);
        API_DEF_ADD_QUERY_PARAMS(String, "pricedTimeTo",
            ZH_WORDS_GETTER("inpatientbilling.field.pricedTimeTo"), "2026-07-24 23:59:59", false);
        info->responses[Status::CODE_200].addExample(
            "success", ListPricedOrdersJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/inpatientbilling/list-priced-orders",
        listPricedOrders,
        ListPricedOrdersQuery,
        execListPricedOrders(query));

    // 4. List supplement parent orders (GET)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.list-supplement-parent-orders.summary"),
        listSupplementParentOrders,
        ListSupplementParentOrdersJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "encounterIds",
            ZH_WORDS_GETTER("inpatientbilling.field.encounterIds"),
            "337907884548657152,337907884548657153", true);
        API_DEF_ADD_QUERY_PARAMS(String, "therapyType",
            ZH_WORDS_GETTER("inpatientbilling.field.therapyType"), "TEMPORARY", false);
        API_DEF_ADD_QUERY_PARAMS(String, "keyword",
            ZH_WORDS_GETTER("inpatientbilling.field.keyword"), ZH_WORDS_GETTER("business-text.sample.medication-keyword"), false);
        API_DEF_ADD_QUERY_PARAMS(String, "orgId",
            ZH_WORDS_GETTER("inpatientbilling.field.orgId"), "327170091059744768", false);
        info->responses[Status::CODE_200].addExample(
            "success", ListSupplementParentOrdersJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/inpatientbilling/list-supplement-parent-orders",
        listSupplementParentOrders,
        ListSupplementParentOrdersQuery,
        execListSupplementParentOrders(query));

    // 5. Confirm supplement pricing (POST)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.confirm-supplement-pricing.summary"),
        confirmSupplementPricing,
        ConfirmSupplementPricingJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", ConfirmSupplementPricingDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", ConfirmSupplementPricingJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/inpatientbilling/confirm-supplement-pricing",
        confirmSupplementPricing,
        BODY_DTO(ConfirmSupplementPricingDTO::Wrapper, dto),
        execConfirmSupplementPricing(dto));

    // 6. Cancel pricing (POST)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.cancel-pricing.summary"),
        cancelPricing,
        CancelPricingJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", CancelPricingDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", CancelPricingJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/inpatientbilling/cancel-pricing",
        cancelPricing,
        BODY_DTO(CancelPricingDTO::Wrapper, dto),
        execCancelPricing(dto));

    // 7. List auto roll configs (GET)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.list-auto-roll-configs.summary"),
        listAutoRollConfigs,
        ListAutoRollConfigsJsonVO::Wrapper,
        API_TAG,
        API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
            ZH_WORDS_GETTER("inpatientbilling.field.encounterId"), "337907884548657152", true);
        API_DEF_ADD_QUERY_PARAMS(String, "patientId",
            ZH_WORDS_GETTER("inpatientbilling.field.patientId"), "326940849401761792", true);
        API_DEF_ADD_QUERY_PARAMS(String, "billDate",
            ZH_WORDS_GETTER("inpatientbilling.field.billDate"), "2026-07-24", false);
        info->responses[Status::CODE_200].addExample(
            "success", ListAutoRollConfigsJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/inpatientbilling/list-auto-roll-configs",
        listAutoRollConfigs,
        ListAutoRollConfigsQuery,
        execListAutoRollConfigs(query));

    // 8. Create auto gen rule (POST)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.create-auto-gen-rule.summary"),
        createAutoGenRule,
        CreateAutoGenRuleJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", CreateAutoGenRuleDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", CreateAutoGenRuleJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/inpatientbilling/create-auto-gen-rule",
        createAutoGenRule,
        BODY_DTO(CreateAutoGenRuleDTO::Wrapper, dto),
        execCreateAutoGenRule(dto));

    // 9. Update auto gen rule (POST)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.update-auto-gen-rule.summary"),
        updateAutoGenRule,
        UpdateAutoGenRuleJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", UpdateAutoGenRuleDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", UpdateAutoGenRuleJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/inpatientbilling/update-auto-gen-rule",
        updateAutoGenRule,
        BODY_DTO(UpdateAutoGenRuleDTO::Wrapper, dto),
        execUpdateAutoGenRule(dto));

    // 10. Delete auto gen rule (POST)
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("inpatientbilling.delete-auto-gen-rule.summary"),
        deleteAutoGenRule,
        DeleteAutoGenRuleJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", DeleteAutoGenRuleDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", DeleteAutoGenRuleJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/inpatientbilling/delete-auto-gen-rule",
        deleteAutoGenRule,
        BODY_DTO(DeleteAutoGenRuleDTO::Wrapper, dto),
        execDeleteAutoGenRule(dto));

    // 11. Enable or disable an auto generation rule (POST)
    API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("inpatientbilling.set-auto-gen-rule-status.summary"),
        activateAutoGenRule,
        ActivateAutoGenRuleJsonVO::Wrapper,
        API_TAG,
        info->body.addExample("default", ActivateAutoGenRuleDTO::createShared());
        info->responses[Status::CODE_200].addExample(
            "success", ActivateAutoGenRuleJsonVO::createShared());
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
		"/inpatientbilling/set-auto-gen-rule-status",
        activateAutoGenRule,
        BODY_DTO(ActivateAutoGenRuleDTO::Wrapper, dto),
        execActivateAutoGenRule(dto));

private:
    PagePricingCatalogJsonVO::Wrapper execPagePricingCatalog(
        const PagePricingCatalogQuery::Wrapper& query);
    ConfirmDirectPricingJsonVO::Wrapper execConfirmDirectPricing(
        const ConfirmDirectPricingDTO::Wrapper& dto);
    ListPricedOrdersJsonVO::Wrapper execListPricedOrders(
        const ListPricedOrdersQuery::Wrapper& query);
    ListSupplementParentOrdersJsonVO::Wrapper execListSupplementParentOrders(
        const ListSupplementParentOrdersQuery::Wrapper& query);
    ConfirmSupplementPricingJsonVO::Wrapper execConfirmSupplementPricing(
        const ConfirmSupplementPricingDTO::Wrapper& dto);
    CancelPricingJsonVO::Wrapper execCancelPricing(
        const CancelPricingDTO::Wrapper& dto);
    ListAutoRollConfigsJsonVO::Wrapper execListAutoRollConfigs(
        const ListAutoRollConfigsQuery::Wrapper& query);
    CreateAutoGenRuleJsonVO::Wrapper execCreateAutoGenRule(
        const CreateAutoGenRuleDTO::Wrapper& dto);
    UpdateAutoGenRuleJsonVO::Wrapper execUpdateAutoGenRule(
        const UpdateAutoGenRuleDTO::Wrapper& dto);
    DeleteAutoGenRuleJsonVO::Wrapper execDeleteAutoGenRule(
        const DeleteAutoGenRuleDTO::Wrapper& dto);
    ActivateAutoGenRuleJsonVO::Wrapper execActivateAutoGenRule(
        const ActivateAutoGenRuleDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_INPATIENT_BILLING_CONTROLLER_
