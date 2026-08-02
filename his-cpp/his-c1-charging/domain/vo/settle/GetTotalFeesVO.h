#pragma once

#ifndef _GET_TOTAL_FEES_VO_
#define _GET_TOTAL_FEES_VO_

#include "../../GlobalInclude.h"
#include "../../dto/settle/GetTotalFeesDTO.h"


#include OATPP_CODEGEN_BEGIN(DTO)

class GetTotalFeesJsonVO : public JsonVO<oatpp::Vector<GetTotalFeesDTO::Wrapper>> {
    DTO_INIT(GetTotalFeesJsonVO, JsonVO<oatpp::Vector<GetTotalFeesDTO::Wrapper>>);
};

#include OATPP_CODEGEN_END(DTO)

#endif