#pragma once
#ifndef _MonthVisit_VO_
#define _MonthVisit_VO_

#include "../../GlobalInclude.h"
#include "../../dto/data_statistics/MonthVisitDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class MonthVisitVO : public JsonVO<MonthVisitDTO::Wrapper> {
	DTO_INIT(MonthVisitVO, JsonVO<MonthVisitDTO::Wrapper>);
};


class MonthVisitPageJsonVO : public JsonVO<MonthVisitPageDTO::Wrapper> {
	DTO_INIT(MonthVisitPageJsonVO, JsonVO<MonthVisitPageDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_MonthVisit_VO_