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
#include "stdafx.h"
#include "InpatientBillingController.h"
#include "service/inpatientbilling/InpatientBillingService.h"

PagePricingCatalogJsonVO::Wrapper InpatientBillingController::execPagePricingCatalog(
    const PagePricingCatalogQuery::Wrapper& query)
{
    return {};
}

ConfirmDirectPricingJsonVO::Wrapper InpatientBillingController::execConfirmDirectPricing(
    const ConfirmDirectPricingDTO::Wrapper& dto)
{
    auto jvo = ConfirmDirectPricingJsonVO::createShared();
    jvo->setStatus(RS_SUCCESS);
    return jvo;
}

ListPricedOrdersJsonVO::Wrapper InpatientBillingController::execListPricedOrders(
    const ListPricedOrdersQuery::Wrapper& query)
{
    return {};
}

ListSupplementParentOrdersJsonVO::Wrapper InpatientBillingController::execListSupplementParentOrders(
    const ListSupplementParentOrdersQuery::Wrapper& query)
{
    return {};
}

ConfirmSupplementPricingJsonVO::Wrapper InpatientBillingController::execConfirmSupplementPricing(
    const ConfirmSupplementPricingDTO::Wrapper& dto)
{
    auto jvo = ConfirmSupplementPricingJsonVO::createShared();
    jvo->setStatus(RS_SUCCESS);
    return jvo;
}

CancelPricingJsonVO::Wrapper InpatientBillingController::execCancelPricing(
    const CancelPricingDTO::Wrapper& dto)
{
    auto jvo = CancelPricingJsonVO::createShared();
    jvo->setStatus(RS_SUCCESS);
    return jvo;
}

ListAutoRollConfigsJsonVO::Wrapper InpatientBillingController::execListAutoRollConfigs(
    const ListAutoRollConfigsQuery::Wrapper& query)
{
    auto result = InpatientBillingService().listAutoRollConfigs(query);
    auto jvo = ListAutoRollConfigsJsonVO::createShared();
    jvo->success(result);
    return jvo;
}

CreateAutoGenRuleJsonVO::Wrapper InpatientBillingController::execCreateAutoGenRule(
    const CreateAutoGenRuleDTO::Wrapper& dto)
{
    auto result = InpatientBillingService().createAutoGenRule(dto);
    auto jvo = CreateAutoGenRuleJsonVO::createShared();
    jvo->success(result);
    return jvo;
}

UpdateAutoGenRuleJsonVO::Wrapper InpatientBillingController::execUpdateAutoGenRule(
    const UpdateAutoGenRuleDTO::Wrapper& dto)
{
    auto result = InpatientBillingService().updateAutoGenRule(dto);
    auto jvo = UpdateAutoGenRuleJsonVO::createShared();
    jvo->success(result);
    return jvo;
}

DeleteAutoGenRuleJsonVO::Wrapper InpatientBillingController::execDeleteAutoGenRule(
    const DeleteAutoGenRuleDTO::Wrapper& dto)
{
    auto result = InpatientBillingService().deleteAutoGenRule(dto);
    auto jvo = DeleteAutoGenRuleJsonVO::createShared();
    jvo->success(result);
    return jvo;
}

ActivateAutoGenRuleJsonVO::Wrapper InpatientBillingController::execActivateAutoGenRule(
    const ActivateAutoGenRuleDTO::Wrapper& dto)
{
    auto result = InpatientBillingService().activateAutoGenRule(dto);
    auto jvo = ActivateAutoGenRuleJsonVO::createShared();
    jvo->success(result);
    return jvo;
}
