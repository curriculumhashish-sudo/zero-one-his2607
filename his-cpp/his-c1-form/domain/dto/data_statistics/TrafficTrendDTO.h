#pragma once

#ifndef _TRAFFIC_TREND_DTO_
#define _TRAFFIC_TREND_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)



class TrafficTrendDTO : public oatpp::DTO
{
	DTO_INIT(TrafficTrendDTO, DTO);

	
	DTO_FIELD(String, xLabel);
	DTO_FIELD_INFO(xLabel) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.x_label");
	}

	
	DTO_FIELD(Int32, visitCount);
	DTO_FIELD_INFO(visitCount) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.visit_count");
	}

};



class TrafficTrendPageDTO : public PageDTO<TrafficTrendDTO::Wrapper>
{
	DTO_INIT(TrafficTrendPageDTO, PageDTO<TrafficTrendDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_TRAFFIC_TREND_DTO_