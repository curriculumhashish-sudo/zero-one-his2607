#pragma once

#ifndef _MONTH_VISIT_DTO_
#define _MONTH_VISIT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)



class MonthVisitDTO : public oatpp::DTO
{
	DTO_INIT(MonthVisitDTO, DTO);

	
	DTO_FIELD(String, month);
	DTO_FIELD_INFO(month) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.month");
	}


	DTO_FIELD(Int32, totalVisit);
	DTO_FIELD_INFO(totalVisit) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.total_visit");
	}

};



class MonthVisitPageDTO : public PageDTO<MonthVisitDTO::Wrapper>
{
	DTO_INIT(MonthVisitPageDTO, PageDTO<MonthVisitDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !_MONTH_VISIT_DTO_