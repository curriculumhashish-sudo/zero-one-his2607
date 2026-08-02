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
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.printType");
        info->required = true;
    }

    // 是否设为默认
    DTO_FIELD(Boolean, isDefault);
    DTO_FIELD_INFO(isDefault) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.isDefault");
        info->required = false;
    }
    // 打印追踪ID
    DTO_FIELD(String, traceId);
    DTO_FIELD_INFO(traceId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.traceId");
        info->required = true;
    }

    // 绑定代码
    DTO_FIELD(String, bindingCode);
    DTO_FIELD_INFO(bindingCode) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.bindingCode");
        info->required = true;
    }

    // 模板版本
    DTO_FIELD(Int32, templateVersion);
    DTO_FIELD_INFO(templateVersion) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.templateVersion");
        info->required = true;
    }

    // 打印机名称
    DTO_FIELD(String, printerName);
    DTO_FIELD_INFO(printerName) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.printerName");
        info->required = true;
    }

    // 打印份数
    DTO_FIELD(Int32, copies);
    DTO_FIELD_INFO(copies) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.copies");
        info->required = true;
    }
    // 操作员ID
    DTO_FIELD(String, operatorId);
    DTO_FIELD_INFO(operatorId) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.operatorId");
        info->required = true;
    }

    // 操作员名称
    DTO_FIELD(String, operatorName);
    DTO_FIELD_INFO(operatorName) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.operatorName");
        info->required = true;
    }

    // 客户端类型
    DTO_FIELD(String, clientType);
    DTO_FIELD_INFO(clientType) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.clientType");
        info->required = true;
    }

    // 业务参考JSON
    DTO_FIELD(String, bizRefJson);
    DTO_FIELD_INFO(bizRefJson) {
        info->description = ZH_WORDS_GETTER("macchiato.charging.register.bizRefJson");
        info->required = true;
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_REGISTER_RECEIPT_DTO_
