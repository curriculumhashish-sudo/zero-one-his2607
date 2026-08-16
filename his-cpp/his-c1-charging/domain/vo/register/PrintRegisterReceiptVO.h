#pragma once
#ifndef _PRINT_REGISTER_RECEIPT_VO_
#define _PRINT_REGISTER_RECEIPT_VO_
#include "../../GlobalInclude.h"

#include "../../dto/register/PrintRegisterReceiptDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Print register receipt response VO
 */
class PrintRegisterReceiptVO : public NoDataJsonVO {
    DTO_INIT(PrintRegisterReceiptVO, NoDataJsonVO);

    // Print request data
    DTO_FIELD(Object<PrintRegisterReceiptDTO>, printRequest);
    DTO_FIELD_INFO(printRequest) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.printRequest");
        info->required = false;
    }

    // Whether printing succeeded
    DTO_FIELD(Boolean, printSuccess);
    DTO_FIELD_INFO(printSuccess) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.printSuccess");
        info->required = true;
    }

    // Failure message (if print failed)
    DTO_FIELD(String, errorMessage);
    DTO_FIELD_INFO(errorMessage) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.printErrorMessage");
        info->required = false;
    }

    // Printer status
    DTO_FIELD(String, printerStatus);
    DTO_FIELD_INFO(printerStatus) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.printerStatus");
        info->required = false;
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_REGISTER_RECEIPT_VO_