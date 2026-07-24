#pragma once

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientPayRecordsQuery :public PageQuery
{
	DTO_INIT(Pay_ManageGetPatientPayRecordsQuery, PageQuery);
};

#include OATPP_CODEGEN_END(DTO)