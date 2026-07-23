#pragma once

#ifndef _PREPAYMENT_VO_
#define _PREPAYMENT_VO_

#include "../../GlobalInclude.h"
#include "domain/dto/pay_manage/PrepaymentDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
   收预交金响应视图对象
*/
class PrepaymentAddVO : public JsonVO<PrepaymentAddResponseDTO::Wrapper>
{
    DTO_INIT(PrepaymentAddVO, JsonVO<PrepaymentAddResponseDTO::Wrapper>);
};

/*
   退预交金响应视图对象
*/
class PrepaymentRefundVO : public JsonVO<PrepaymentRefundResponseDTO::Wrapper>
{
    DTO_INIT(PrepaymentRefundVO, JsonVO<PrepaymentRefundResponseDTO::Wrapper>);
};

/*
   预交金分页查询响应视图对象
*/
class PrepaymentPageJsonVO : public JsonVO<PrepaymentRecordDTO::Wrapper>
{
    DTO_INIT(PrepaymentPageJsonVO, JsonVO<PrepaymentRecordDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPAYMENT_VO_
