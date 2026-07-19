#pragma once
#ifndef _PRINT_REGISTER_RECEIPT_DTO_
#define _PRINT_REGISTER_RECEIPT_DTO_
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 打印挂号小票DTO
 */
class PrintRegisterReceiptDTO : public oatpp::DTO {
    DTO_INIT(PrintRegisterReceiptDTO, DTO);

    // 打印类型
    DTO_FIELD(String, printType);
    DTO_FIELD_INFO(printType) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.printType");
        info->required = true;
    }

    // 是否设为默认
    DTO_FIELD(Boolean, isDefault);
    DTO_FIELD_INFO(isDefault) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.field.isDefault");
        info->required = false;
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_REGISTER_RECEIPT_DTO_
