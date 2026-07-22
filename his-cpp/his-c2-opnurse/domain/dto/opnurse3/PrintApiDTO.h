#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/21
*/
#ifndef _PRINT_API_DTO_
#define _PRINT_API_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// POST /print/print request
class PrintRequestDTO : public oatpp::DTO
{
	DTO_INIT(PrintRequestDTO, DTO);
	// ids to print
	DTO_FIELD(List<String>, ids);
	DTO_FIELD_INFO(ids) { info->description = ZH_WORDS_GETTER("print.field.printIds"); }
	// is reprint
	DTO_FIELD(Boolean, reprint);
	DTO_FIELD_INFO(reprint) { info->description = ZH_WORDS_GETTER("print.field.reprint"); }
};

// POST /print/print response
class PrintResultDTO : public oatpp::DTO
{
	DTO_INIT(PrintResultDTO, DTO);
	DTO_FIELD(Boolean, success);
	DTO_FIELD_INFO(success) { info->description = ZH_WORDS_GETTER("print.field.printSuccess"); }
	DTO_FIELD(String, printer);
	DTO_FIELD_INFO(printer) { info->description = ZH_WORDS_GETTER("print.field.printerName"); }
	DTO_FIELD(String, message);
	DTO_FIELD_INFO(message) { info->description = ZH_WORDS_GETTER("print.field.printMsg"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_API_DTO_
