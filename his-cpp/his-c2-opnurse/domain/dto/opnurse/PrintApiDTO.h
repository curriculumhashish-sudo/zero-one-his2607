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

// POST /opnurse/print request
class PrintRequestDTO : public oatpp::DTO
{
	DTO_INIT(PrintRequestDTO, DTO);
	// ids — must be non-empty
	DTO_FIELD(List<String>, ids);
	DTO_FIELD_INFO(ids) {
		info->description = ZH_WORDS_GETTER("opnurse3.field.printIds");
		info->required = true;
	}
	// docType — one of: treatment/prescription/charge_receipt/reg_receipt/infusion_label/daily_settlement/exam_request/lab_request
	DTO_FIELD(String, docType);
	DTO_FIELD_INFO(docType) {
		info->description = ZH_WORDS_GETTER("opnurse3.field.docType");
		info->required = true;
	}
	// reprint — false=first print, true=reprint
	DTO_FIELD(Boolean, reprint);
	DTO_FIELD_INFO(reprint) {
		info->description = ZH_WORDS_GETTER("opnurse3.field.reprint");
	}
	// data — print payload, specific fields vary by docType
	DTO_FIELD(PrintDataDTO::Wrapper, data);
	DTO_FIELD_INFO(data) {
		info->description = ZH_WORDS_GETTER("opnurse3.field.printData");
		info->required = true;
	}
};

// POST /opnurse/print response
class PrintResultDTO : public oatpp::DTO
{
	DTO_INIT(PrintResultDTO, DTO);
	DTO_FIELD(Boolean, success);
	DTO_FIELD_INFO(success) { info->description = ZH_WORDS_GETTER("opnurse3.field.printSuccess"); }
	DTO_FIELD(String, printer);
	DTO_FIELD_INFO(printer) { info->description = ZH_WORDS_GETTER("opnurse3.field.printerName"); }
	DTO_FIELD(String, message);
	DTO_FIELD_INFO(message) { info->description = ZH_WORDS_GETTER("opnurse3.field.printMsg"); }
	DTO_FIELD(String, pdfBase64);
	DTO_FIELD_INFO(pdfBase64) { info->description = ZH_WORDS_GETTER("opnurse3.field.pdfBase64"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_API_DTO_
