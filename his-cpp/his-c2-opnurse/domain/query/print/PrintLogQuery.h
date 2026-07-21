#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/21
*/
#ifndef _PRINT_LOG_QUERY_
#define _PRINT_LOG_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PrintLogQuery : public PageQuery
{
	DTO_INIT(PrintLogQuery, PageQuery);
	DTO_FIELD(String, docType);
	DTO_FIELD_INFO(docType) { info->description = ZH_WORDS_GETTER("print.field.docType"); }
	DTO_FIELD(String, pname);
	DTO_FIELD_INFO(pname) { info->description = ZH_WORDS_GETTER("print.field.patientName"); }
	DTO_FIELD(Int32, isReprint);
	DTO_FIELD_INFO(isReprint) { info->description = ZH_WORDS_GETTER("print.field.isReprint"); }
	DTO_FIELD(String, timefrom);
	DTO_FIELD_INFO(timefrom) { info->description = ZH_WORDS_GETTER("print.field.timefrom"); }
	DTO_FIELD(String, timeto);
	DTO_FIELD_INFO(timeto) { info->description = ZH_WORDS_GETTER("print.field.timeto"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_LOG_QUERY_
