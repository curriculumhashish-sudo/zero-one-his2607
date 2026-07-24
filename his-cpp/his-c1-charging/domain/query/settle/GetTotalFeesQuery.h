#pragma once
#ifndef _GET_TOTAL_FEES_QUERY_
#define _GET_TOTAL_FEES_QUERY_

// 1. 先引入全局头文件，加载API_DTO_FIELD、ZH_WORDS_GETTER等自定义宏
#include "../../GlobalInclude.h"
// 2. 再开启Oatpp DTO代码生成上下文
#include OATPP_CODEGEN_BEGIN(DTO)

class GetTotalFeesQuery : public oatpp::DTO
{
    DTO_INIT(GetTotalFeesQuery, DTO);
    // 单行写法规避换行解析bug
    API_DTO_FIELD(oatpp::String, inpatient_no, ZH_WORDS_GETTER("qi.charging.settle.inpatient_no"), true, "");
};

#include OATPP_CODEGEN_END(DTO)
#endif