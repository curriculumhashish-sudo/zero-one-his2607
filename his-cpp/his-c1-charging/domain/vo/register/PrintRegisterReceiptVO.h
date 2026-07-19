#pragma once
#ifndef _PRINT_REGISTER_RECEIPT_VO_
#define _PRINT_REGISTER_RECEIPT_VO_
#include "../../GlobalInclude.h"
#include "../../../domain/dto/register/PrintRegisterReceiptDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * ¥Ú”°π“∫≈–°∆±VO
 */
class PrintRegisterReceiptVO : public JsonVO <PrintRegisterReceiptDTO::Wrapper> {
    DTO_INIT(PrintRegisterReceiptVO, JsonVO <PrintRegisterReceiptDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_REGISTER_RECEIPT_VO_
