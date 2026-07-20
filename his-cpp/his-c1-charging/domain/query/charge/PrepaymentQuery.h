#pragma once

#ifndef _PREPAYMENT_QUERY_
#define _PREPAYMENT_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
   预交金流水查询参数
*/
class PrepaymentQuery : public oatpp::DTO
{
    DTO_INIT(PrepaymentQuery, DTO);

    // 住院号
    API_DTO_FIELD(String, inpatientNo,
        ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentQuery.inpatientNo.description"),
        true, "ZY202607190001");
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPAYMENT_QUERY_
