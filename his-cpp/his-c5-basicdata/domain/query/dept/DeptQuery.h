#pragma once

#ifndef _DEPT_QUERY_
#define _DEPT_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DeptDeleteQuery : public oatpp::DTO
{
    DTO_INIT(DeptDeleteQuery, DTO);

    // 科室编号
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("dept.field.id"), true)

    // 关联一个PayloadDTO负载数据对象
    CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);

public:
    std::string validate() const
    {
        if (!id || id->empty())
            return "id is required.";
        return "";
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DEPT_QUERY_