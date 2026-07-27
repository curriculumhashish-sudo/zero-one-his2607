#pragma once 
#ifndef _WARDTREE_QUERY_H_
#define _WARDTREE_QUERY_H_
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class WardTreeQuery :public oatpp::DTO {
    DTO_INIT(WardTreeQuery, DTO);

    DTO_FIELD(String, organizationId);
    DTO_FIELD(String, wardId);
    DTO_FIELD(String, parentId);
    DTO_FIELD(String, keyword);
    DTO_FIELD(String, statusEnum);




};
#include OATPP_CODEGEN_END(DTO)
#endif // !_WARDTREE_QUERY_H_
