#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/21
*/
#ifndef _PRINT_API_DTO_
#define _PRINT_API_DTO_
#include "../../GlobalInclude.h"
#include "PrintDataDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// POST /opnurse3/print request
class PrintRequestDTO : public oatpp::DTO
{
	DTO_INIT(PrintRequestDTO, DTO);
	DTO_FIELD(List<String>, ids);
	DTO_FIELD_INFO(ids) { info->description = ZH_WORDS_GETTER("print.field.printIds"); }
	DTO_FIELD(String, docType);
	DTO_FIELD_INFO(docType) { info->description = ZH_WORDS_GETTER("print.field.docType"); }
	DTO_FIELD(Boolean, reprint);
	DTO_FIELD_INFO(reprint) { info->description = ZH_WORDS_GETTER("print.field.reprint"); }
	DTO_FIELD(PrintDataDTO::Wrapper, data);
	DTO_FIELD_INFO(data) { info->description = ZH_WORDS_GETTER("print.field.printData"); }
};

// POST /opnurse3/print response
class PrintResultDTO : public oatpp::DTO
{
	DTO_INIT(PrintResultDTO, DTO);
	DTO_FIELD(Boolean, success);
	DTO_FIELD_INFO(success) { info->description = ZH_WORDS_GETTER("print.field.printSuccess"); }
	DTO_FIELD(String, printer);
	DTO_FIELD_INFO(printer) { info->description = ZH_WORDS_GETTER("print.field.printerName"); }
	DTO_FIELD(String, message);
	DTO_FIELD_INFO(message) { info->description = ZH_WORDS_GETTER("print.field.printMsg"); }
	DTO_FIELD(String, pdfBase64);
	DTO_FIELD_INFO(pdfBase64) { info->description = ZH_WORDS_GETTER("print.field.pdfBase64"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_API_DTO_
