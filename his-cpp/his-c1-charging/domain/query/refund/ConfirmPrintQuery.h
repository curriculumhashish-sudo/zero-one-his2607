#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _CONFIRM_PRINT_QUERY_
#define _CONFIRM_PRINT_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Query params for confirmPrint GET endpoint
// Passed as query string from electron-hiprint client after printing
class ConfirmPrintQuery : public oatpp::DTO
{
    DTO_INIT(ConfirmPrintQuery, DTO);

    // Print job trace ID
    DTO_FIELD(String, traceId);
    DTO_FIELD_INFO(traceId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.traceId");
    };

    // Business number (e.g., "RT202606260005")
    DTO_FIELD(String, busNo);
    DTO_FIELD_INFO(busNo) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.busNo");
    };

    // Scenario code (e.g., "op-refund-print")
    DTO_FIELD(String, scenarioCode);
    DTO_FIELD_INFO(scenarioCode) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.scenarioCode");
    };

    // Template version number
    DTO_FIELD(Int32, templateVersion);
    DTO_FIELD_INFO(templateVersion) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.templateVersion");
    };

    // Print status (e.g., "SUCCESS")
    DTO_FIELD(String, status);
    DTO_FIELD_INFO(status) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.status");
    };

    // Actual print data (JSON string)
    DTO_FIELD(String, printData);
    DTO_FIELD_INFO(printData) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.printData");
    };

    // Business reference (JSON string)
    DTO_FIELD(String, bizRef);
    DTO_FIELD_INFO(bizRef) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.bizRef");
    };

    // Number of copies printed
    DTO_FIELD(Int32, copies);
    DTO_FIELD_INFO(copies) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.copies");
    };

    // Client type (e.g., "electron-hiprint")
    DTO_FIELD(String, clientType);
    DTO_FIELD_INFO(clientType) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.clientType");
    };
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_CONFIRM_PRINT_QUERY_
