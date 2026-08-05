/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/26 00:00:00

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
#include "stdafx.h"
#include "RefundPreparePrintService.h"
#include "../../dao/refund/RefundPreparePrintDAO.h"
#include "id/UuidFacade.h"
#include <sstream>
#include <ctime>
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

// Prevent macro conflicts
#include "NacosClient.h"

// Prepare print template for refund receipt
RefundPreparePrintDTO::Wrapper RefundPreparePrintService::prepare(const RefundPreparePrintQuery::Wrapper& query)
{
    auto dto = RefundPreparePrintDTO::createShared();

    std::string scenarioCode = query->scenarioCode.getValue("");
    dto->scenarioCode = scenarioCode;

    // Generate trace ID (with dashes) and business number
    UuidFacade uuid;
    dto->traceId = uuid.genUuid(false);

    time_t now = time(nullptr);
    struct tm tmInfo;
#ifdef LINUX
    localtime_r(&now, &tmInfo);
#else
    localtime_s(&tmInfo, &now);
#endif
    char dateBuf[16];
    strftime(dateBuf, sizeof(dateBuf), "%Y%m%d", &tmInfo);
    std::ostringstream busNoStream;
    busNoStream << "RT" << dateBuf
        << std::setfill('0') << std::setw(4) << (tmInfo.tm_hour * 100 + tmInfo.tm_min);
    dto->busNo = busNoStream.str();

    // Query template from database
    RefundPreparePrintDAO dao;
    auto session = dao.getSqlSession();

    // First check if template exists
    SqlParams pCount;
    std::ostringstream sqlCount;
    sqlCount << "SELECT COUNT(*) FROM prt_template"
        << " WHERE scenario_code = ? AND is_deleted = 0";
    SQLPARAMS_PUSH(pCount, "s", std::string, scenarioCode);
    uint64_t tplCount = session->executeQueryNumerical(sqlCount.str(), pCount);

    if (tplCount > 0) {
        // Fetch print_json as raw string using ResultSet
        // We'll use executeQuery to get the JSON string
        std::ostringstream sqlTpl;
        sqlTpl << "SELECT version, COALESCE(print_json, '{}') AS print_json,"
            << " COALESCE(sample_print_data, '') AS sample_print_data"
            << " FROM prt_template"
            << " WHERE scenario_code = ? AND is_deleted = 0"
            << " ORDER BY version DESC LIMIT 1";

        SqlParams pTpl;
        SQLPARAMS_PUSH(pTpl, "s", std::string, scenarioCode);

        // Execute query manually to get version + JSON strings
        auto conn = session->getConnection();
        auto pstmt = conn->prepareStatement(sqlTpl.str());
        // Bind param
        pstmt->setString(1, scenarioCode);
        auto res = pstmt->executeQuery();
        if (res->next()) {
            dto->templateVersion = res->getInt("version");
            std::string printJsonStr = res->getString("print_json");
            std::string sampleDataStr = res->getString("sample_print_data");

            // Deserialize print_json into RefundPrintJsonDTO
            if (!printJsonStr.empty() && printJsonStr != "{}") {
                auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
                dto->PrintJson = mapper->readFromString<oatpp::Object<RefundPrintJsonDTO>>(printJsonStr);
            }

            // Set sample data if not empty
            if (!sampleDataStr.empty()) {
                dto->samplePrintData = sampleDataStr;
            }
        }
        delete res;
        delete pstmt;
    } else {
        dto->templateVersion = 1;
    }

    // Insert print event log (status=PENDING)
    std::ostringstream sqlLog;
    sqlLog << "INSERT INTO prt_print_event_log"
        << " (id, tenant_id, trace_id, bus_no, binding_code, template_version, status, created_at)"
        << " VALUES (?, '1', ?, ?, ?, ?, 'PENDING', NOW())";
    UuidFacade logUuid;
    SqlParams pLog;
    SQLPARAMS_PUSH(pLog, "s", std::string, logUuid.genUuid());
    SQLPARAMS_PUSH(pLog, "s", std::string, dto->traceId.getValue(""));
    SQLPARAMS_PUSH(pLog, "s", std::string, dto->busNo.getValue(""));
    SQLPARAMS_PUSH(pLog, "s", std::string, scenarioCode);
    SQLPARAMS_PUSH(pLog, "i", int, (int)dto->templateVersion.getValue(1));
    session->executeUpdate(sqlLog.str(), pLog);

    return dto;
}
