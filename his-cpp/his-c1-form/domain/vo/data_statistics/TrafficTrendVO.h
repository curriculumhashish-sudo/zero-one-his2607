#pragma once
#ifndef _TrafficTrend_VO_
#define _TrafficTrend_VO_

#include "../../GlobalInclude.h"
#include "../../dto/data_statistics/TrafficTrendDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class TrafficTrendVO : public JsonVO<TrafficTrendDTO::Wrapper> {
	DTO_INIT(TrafficTrendVO, JsonVO<TrafficTrendDTO::Wrapper>);
};


class TrafficTrendPageJsonVO : public JsonVO<TrafficTrendPageDTO::Wrapper> {
	DTO_INIT(TrafficTrendPageJsonVO, JsonVO<TrafficTrendPageDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_TrafficTrend_VO_