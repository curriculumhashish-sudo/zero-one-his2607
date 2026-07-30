#pragma once

#ifndef _PREPARE_PRINT_DTO_
#define _PREPARE_PRINT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   准备打印请求
*/
class PreparePrintRequestDTO : public oatpp::DTO
{
    DTO_INIT(PreparePrintRequestDTO, DTO);

    // 业务场景码（如 "op-charge-print"）
    API_DTO_FIELD(String, scenarioCode, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintRequestDTO.scenarioCode.description"), false, "op-charge-print");
};

/*
   准备打印响应
*/
class PreparePrintResponseDTO : public oatpp::DTO
{
    DTO_INIT(PreparePrintResponseDTO, DTO);

    // 打印追踪 ID（贯穿全链路）
    API_DTO_FIELD(String, traceId, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.traceId.description"), true, "");

    // 模板业务号
    API_DTO_FIELD(String, busNo, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.busNo.description"), true, "");

    // 场景码
    API_DTO_FIELD(String, scenarioCode, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.scenarioCode.description"), true, "op-charge-print");

    // 模板版本号
    API_DTO_FIELD(Int32, templateVersion, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.templateVersion.description"), true, 2);

    // 模板结构（原始JSON对象）
    API_DTO_FIELD(Any, PrintJson, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.PrintJson.description"), true, {});

    // 示例数据（原始JSON，调试用）
    API_DTO_FIELD(Any, samplePrintData, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.samplePrintData.description"), false, {});
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPARE_PRINT_DTO_
