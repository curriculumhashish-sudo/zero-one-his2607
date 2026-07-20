#pragma once

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientPayRecordsQuery :public PageQuery
{
	DTO_INIT(GetPatientPayRecordsQuery, PageQuery);
};

#include OATPP_CODEGEN_END(DTO)