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
    API_DTO_FIELD_DEFAULT(String, scenarioCode, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintRequestDTO.scenarioCode"));
};

/*
   准备打印响应
*/
class PreparePrintResponseDTO : public oatpp::DTO
{
    DTO_INIT(PreparePrintResponseDTO, DTO);

    // 打印追踪 ID（贯穿全链路）
    API_DTO_FIELD_DEFAULT(String, traceId, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.traceId"));

    // 模板业务号
    API_DTO_FIELD_DEFAULT(String, busNo, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.busNo"));

    // 场景码
    API_DTO_FIELD_DEFAULT(String, scenarioCode, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.scenarioCode"));

    // 模板版本号
    API_DTO_FIELD_DEFAULT(Int32, templateVersion, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.templateVersion"));

    // 模板结构（panels / paper / printElements 等）
    API_DTO_FIELD_DEFAULT(Any, PrintJson, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.PrintJson"));

    // 示例数据（调试用）
    API_DTO_FIELD_DEFAULT(Any, samplePrintData, ZH_WORDS_GETTER("lhfc.charging.charge.PreparePrintResponseDTO.samplePrintData"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPARE_PRINT_DTO_
