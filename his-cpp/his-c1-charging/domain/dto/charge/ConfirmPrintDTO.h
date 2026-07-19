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
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.traceId"),
        true, "");

    // 模板业务号（4B 返回）
    API_DTO_FIELD(String, busNo,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.busNo"),
        true, "");

    // 场景码（固定值）
    API_DTO_FIELD_DEFAULT(String, scenarioCode, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.scenarioCode"));

    // 模板版本号（4B 返回）
    API_DTO_FIELD_DEFAULT(Int32, templateVersion, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.templateVersion"));

    // 打印结果 SUCCESS / FAILED
    API_DTO_FIELD(String, status,
        ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.status"),
        true, "");

    // 实际打印的业务数据
    API_DTO_FIELD_DEFAULT(Any, printData, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.printData"));

    // 业务锚点
    API_DTO_FIELD_DEFAULT(Any, bizRef, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.bizRef"));

    // 打印机名称
    API_DTO_FIELD_DEFAULT(String, printer, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.printer"));

    // 打印份数
    API_DTO_FIELD_DEFAULT(Int32, copies, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.copies"));

    // 客户端类型
    API_DTO_FIELD_DEFAULT(String, clientType, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.clientType"));

    // 失败时的错误码
    API_DTO_FIELD_DEFAULT(String, errorCode, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.errorCode"));

    // 失败时的错误信息
    API_DTO_FIELD_DEFAULT(String, errorMessage, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintRequestDTO.errorMessage"));
};

/*
   确认打印响应
*/
class ConfirmPrintResponseDTO : public oatpp::DTO
{
    DTO_INIT(ConfirmPrintResponseDTO, DTO);

    // 审计落库是否成功
    API_DTO_FIELD_DEFAULT(Boolean, ok, ZH_WORDS_GETTER("lhfc.charging.charge.ConfirmPrintResponseDTO.ok"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CONFIRM_PRINT_DTO_
