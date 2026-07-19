#pragma once
#ifndef _PRINT_REGISTER_RECEIPT_CONTROLLER_H_
#define _PRINT_REGISTER_RECEIPT_CONTROLLER_H_
#include "../../domain/dto/register/PrintRegisterReceiptDTO.h"
#include "../../domain/vo/register/PrintRegisterReceiptVO.h"
#include "../../../lib-oatpp/include/domain/vo/BaseJsonVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("macchiato.charging.register.tag.tag3")

class PrintRegisterReceiptController : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(PrintRegisterReceiptController);
public:
    // 打印挂号小票接口描述（Swagger文档）
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("macchiato.charging.register.summary3"), 
        printRegisterReceipt,                            
        PrintRegisterReceiptVO::Wrapper,                  
        API_TAG                                          
    );

    // 打印挂号小票接口端点（路由、参数、处理逻辑）
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,                                       // HTTP方法：POST
        "/print/register-receipt",                         // 路由路径
        printRegisterReceipt,                             // 端点名称
        BODY_DTO(PrintRegisterReceiptDTO::Wrapper, dto),    // 请求体参数（新的DTO，无visitNumber）
        execPrintRegisterReceipt(dto, authObject->getPayload()) // 处理方法（传入DTO和用户身份）
    );

private:
    // 执行打印挂号小票的核心逻辑（私有方法）
    PrintRegisterReceiptVO::Wrapper execPrintRegisterReceipt(const PrintRegisterReceiptDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_PRINT_REGISTER_RECEIPT_CONTROLLER_H_

