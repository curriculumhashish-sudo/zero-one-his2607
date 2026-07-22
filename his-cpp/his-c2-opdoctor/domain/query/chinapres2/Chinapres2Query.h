#ifndef _CHINAPRES2QUERY_H_
#define _CHINAPRES2QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Chinapres2Query : public PageQuery
{
    DTO_INIT(Chinapres2Query, PageQuery);
    //历史处方的位数
    API_DTO_FIELD_REQUIRE(Int32, preNum, ZH_WORDS_GETTER("chinapres2.field.preNum"), true);
    //历史处方的药材
    API_DTO_FIELD_REQUIRE(String, medicine, ZH_WORDS_GETTER("chinapres2.field.medicine"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHINAPRES2QUERY_H_