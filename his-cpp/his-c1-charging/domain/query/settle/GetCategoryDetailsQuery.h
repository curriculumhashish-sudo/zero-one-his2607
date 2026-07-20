#pragma once
#ifndef _GET_CATEGORY_DETAILS_QUERY_
#define _GET_CATEGORY_DETAILS_QUERY_

#include "../../GlobalInclude.h" // 根据你的实际路径调整
#include OATPP_CODEGEN_BEGIN(DTO)

class GetCategoryDetailsQuery : public oatpp::DTO {
	DTO_INIT(GetCategoryDetailsQuery, DTO)

		// 对应 Controller 里定义的查询参数
		DTO_FIELD(String, inpatient_no);
	DTO_FIELD(String, encounter_id);
};

#include OATPP_CODEGEN_END(DTO)
#endif


