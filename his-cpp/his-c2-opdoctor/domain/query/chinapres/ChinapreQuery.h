#ifndef _CHINAPREQUERY_H_
#define _CHINAPREQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapreQuery : public PageQuery
{
    DTO_INIT(ChinapreQuery, PageQuery);
    //历史处方的位数
    API_DTO_FIELD_REQUIRE(Int32, preNum, ZH_WORDS_GETTER("prescription.field.preNum"), true);
    //历史处方的药材
    API_DTO_FIELD_REQUIRE(String, medicine, ZH_WORDS_GETTER("prescription.field.medicine"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CHINAPREQUERY_H_