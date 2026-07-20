#pragma once

#ifndef _CONFIRM_PRINT_DTO_
#define _CONFIRM_PRINT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   确认打印请求（审计落库）
*/
class ConfirmPrintRequestDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmPrintRequestDTO, DTO);

    // 打印追踪 ID（4B 返回）
    API_DTO_FIELD(String, traceId,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.traceId.description"),
        true, "");

    // 模板业务号（4B 返回）
    API_DTO_FIELD(String, busNo,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.busNo.description"),
        true, "");

    // 场景码（固定值）
    API_DTO_FIELD(String, scenarioCode,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.scenarioCode.description"),
        false, "op-charge-print");

    // 模板版本号（4B 返回）
    API_DTO_FIELD(Int32, templateVersion,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.templateVersion.description"),
        false, 0);

    // 打印结果 SUCCESS / FAILED
    API_DTO_FIELD(String, status,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.status.description"),
        true, "");

    // 实际打印的业务数据
    API_DTO_FIELD(Any, printData,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.printData.description"),
        false, {});

    // 业务锚点
    API_DTO_FIELD(Any, bizRef,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.bizRef.description"),
        false, {});

    // 打印机名称
    API_DTO_FIELD(String, printer,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.printer.description"),
        false, "");

    // 打印份数
    API_DTO_FIELD(Int32, copies,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.copies.description"),
        false, 1);

    // 客户端类型
    API_DTO_FIELD(String, clientType,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.clientType.description"),
        false, "electron-hiprint");

    // 失败时的错误码
    API_DTO_FIELD(String, errorCode,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.errorCode.description"),
        false, "");

    // 失败时的错误信息
    API_DTO_FIELD(String, errorMessage,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.errorMessage.description"),
        false, "");
};

/*
   确认打印响应
*/
class ConfirmPrintResponseDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmPrintResponseDTO, DTO);

    // 审计落库是否成功
    API_DTO_FIELD(Boolean, ok, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintResponseDTO.ok.description"), true, true);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CONFIRM_PRINT_DTO_
