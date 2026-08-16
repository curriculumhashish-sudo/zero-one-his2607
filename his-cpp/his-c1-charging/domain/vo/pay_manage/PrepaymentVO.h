#pragma once

#ifndef _PREPAYMENT_VO_
#define _PREPAYMENT_VO_

#include "../../GlobalInclude.h"
#include "domain/dto/pay_manage/PrepaymentDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Add prepayment response view object
class PrepaymentAddVO : public JsonVO<PrepaymentAddResponseDTO::Wrapper>
{
    DTO_INIT(PrepaymentAddVO, JsonVO<PrepaymentAddResponseDTO::Wrapper>);
};

// Refund prepayment response view object
class PrepaymentRefundVO : public JsonVO<PrepaymentRefundResponseDTO::Wrapper>
{
    DTO_INIT(PrepaymentRefundVO, JsonVO<PrepaymentRefundResponseDTO::Wrapper>);
};

// Prepayment page query response view object
class PrepaymentPageJsonVO : public JsonVO<PrepaymentPageDTO::Wrapper>
{
    DTO_INIT(PrepaymentPageJsonVO, JsonVO<PrepaymentPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPAYMENT_VO_
