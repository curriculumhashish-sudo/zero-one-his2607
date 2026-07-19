#pragma once

#ifndef _CHARGE_PRINT_VO_
#define _CHARGE_PRINT_VO_

#include "../../GlobalInclude.h"
#include "domain/dto/charge/PrintChargeReceiptDTO.h"
#include "domain/dto/charge/PreparePrintDTO.h"
#include "domain/dto/charge/ConfirmPrintDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   打印门诊收费小票响应视图对象
*/
class PrintChargeReceiptVO : public JsonVO<PrintChargeReceiptResponseDTO::Wrapper>
{
    DTO_INIT(PrintChargeReceiptVO, JsonVO<PrintChargeReceiptResponseDTO::Wrapper>);
};

/*
   准备打印响应视图对象
*/
class PreparePrintVO : public JsonVO<PreparePrintResponseDTO::Wrapper>
{
    DTO_INIT(PreparePrintVO, JsonVO<PreparePrintResponseDTO::Wrapper>);
};

/*
   确认打印响应视图对象
*/
class ConfirmPrintVO : public JsonVO<ConfirmPrintResponseDTO::Wrapper>
{
    DTO_INIT(ConfirmPrintVO, JsonVO<ConfirmPrintResponseDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CHARGE_PRINT_VO_
