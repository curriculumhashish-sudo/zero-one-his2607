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
#ifndef _INPATIENT_BILLING_VO_H_
#define _INPATIENT_BILLING_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/inpatientbilling/InpatientBillingDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Pricing Catalog page response
class PagePricingCatalogJsonVO : public JsonVO<PagePricingCatalogDTO::Wrapper>
{
    DTO_INIT(PagePricingCatalogJsonVO, JsonVO<PagePricingCatalogDTO::Wrapper>);

public:
    PagePricingCatalogJsonVO()
    {
        this->data = PagePricingCatalogDTO::createShared();
    }
};

// Confirm Direct Pricing response
class ConfirmDirectPricingJsonVO : public JsonVO<ConfirmDirectPricingResponseDTO::Wrapper>
{
    DTO_INIT(ConfirmDirectPricingJsonVO, JsonVO<ConfirmDirectPricingResponseDTO::Wrapper>);

public:
    ConfirmDirectPricingJsonVO()
    {
        this->data = ConfirmDirectPricingResponseDTO::createShared();
    }
};

// List Priced Orders response
class ListPricedOrdersJsonVO : public JsonVO<ListPricedOrdersDTO::Wrapper>
{
    DTO_INIT(ListPricedOrdersJsonVO, JsonVO<ListPricedOrdersDTO::Wrapper>);

public:
    ListPricedOrdersJsonVO()
    {
        this->data = ListPricedOrdersDTO::createShared();
    }
};

// List Supplement Parent Orders response
class ListSupplementParentOrdersJsonVO : public JsonVO<ListSupplementParentOrdersDTO::Wrapper>
{
    DTO_INIT(ListSupplementParentOrdersJsonVO, JsonVO<ListSupplementParentOrdersDTO::Wrapper>);

public:
    ListSupplementParentOrdersJsonVO()
    {
        this->data = ListSupplementParentOrdersDTO::createShared();
    }
};

// Confirm Supplement Pricing response
class ConfirmSupplementPricingJsonVO : public JsonVO<ConfirmSupplementPricingResponseDTO::Wrapper>
{
    DTO_INIT(ConfirmSupplementPricingJsonVO, JsonVO<ConfirmSupplementPricingResponseDTO::Wrapper>);

public:
    ConfirmSupplementPricingJsonVO()
    {
        this->data = ConfirmSupplementPricingResponseDTO::createShared();
    }
};

// Cancel Pricing response
class CancelPricingJsonVO : public JsonVO<CancelPricingResponseDTO::Wrapper>
{
    DTO_INIT(CancelPricingJsonVO, JsonVO<CancelPricingResponseDTO::Wrapper>);

public:
    CancelPricingJsonVO()
    {
        this->data = CancelPricingResponseDTO::createShared();
    }
};

// List Auto Roll Configs response
class ListAutoRollConfigsJsonVO : public JsonVO<ListAutoRollConfigsDTO::Wrapper>
{
    DTO_INIT(ListAutoRollConfigsJsonVO, JsonVO<ListAutoRollConfigsDTO::Wrapper>);

public:
    ListAutoRollConfigsJsonVO()
    {
        this->data = ListAutoRollConfigsDTO::createShared();
    }
};

// Create Auto Gen Rule response
class CreateAutoGenRuleJsonVO : public JsonVO<CreateAutoGenRuleResponseDTO::Wrapper>
{
    DTO_INIT(CreateAutoGenRuleJsonVO, JsonVO<CreateAutoGenRuleResponseDTO::Wrapper>);

public:
    CreateAutoGenRuleJsonVO()
    {
        this->data = CreateAutoGenRuleResponseDTO::createShared();
    }
};

// Update Auto Gen Rule response
class UpdateAutoGenRuleJsonVO : public JsonVO<UpdateAutoGenRuleResponseDTO::Wrapper>
{
    DTO_INIT(UpdateAutoGenRuleJsonVO, JsonVO<UpdateAutoGenRuleResponseDTO::Wrapper>);

public:
    UpdateAutoGenRuleJsonVO()
    {
        this->data = UpdateAutoGenRuleResponseDTO::createShared();
    }
};

// Delete Auto Gen Rule response
class DeleteAutoGenRuleJsonVO : public JsonVO<DeleteAutoGenRuleResponseDTO::Wrapper>
{
    DTO_INIT(DeleteAutoGenRuleJsonVO, JsonVO<DeleteAutoGenRuleResponseDTO::Wrapper>);

public:
    DeleteAutoGenRuleJsonVO()
    {
        this->data = DeleteAutoGenRuleResponseDTO::createShared();
    }
};

// Activate / Suspend Auto Gen Rule response (shared)
class ActivateAutoGenRuleJsonVO : public JsonVO<ActivateAutoGenRuleResponseDTO::Wrapper>
{
    DTO_INIT(ActivateAutoGenRuleJsonVO, JsonVO<ActivateAutoGenRuleResponseDTO::Wrapper>);

public:
    ActivateAutoGenRuleJsonVO()
    {
        this->data = ActivateAutoGenRuleResponseDTO::createShared();
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_INPATIENT_BILLING_VO_H_
