#pragma once

#ifndef _PREPAYMENT_QUERY_
#define _PREPAYMENT_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/*
   预交金流水查询参数
*/
class PrepaymentQuery : public PageQuery
{
    DTO_INIT(PrepaymentQuery, PageQuery);

    // 住院号
    DTO_FIELD(String, inpatientNo);
    DTO_FIELD_INFO(inpatientNo)
    {
        info->description = ZH_WORDS_GETTER("Jack.charging.charge.PrepaymentQuery.inpatientNo.description");
        info->required = true;
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPAYMENT_QUERY_
