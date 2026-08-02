#include "PrintRegisterReceiptService.h"
#include "../../dao/register/PrintRegisterReceiptDAO.h"
#include "../../domain/dto/register/PrintRegisterReceiptDTO.h" 
#include <string>

// 准备打印挂号小票：获取打印模板JSON
std::string PrintRegisterReceiptService::preparePrint(const std::string& scenarioCode) {
    // 调用DAO查询打印模板
    // 场景码使用"REGISTER_RECEIPT"（挂号小票），状态默认"SUCCESS"
    return m_dao.queryPrintTemplate(scenarioCode, "SUCCESS");
}

// 确认打印挂号小票：记录打印日志
int PrintRegisterReceiptService::confirmPrint(const oatpp::Object<PrintRegisterReceiptDTO>& request) {
    // 从请求DTO中提取参数
    std::string traceId = request->traceId;
    std::string bindingCode = request->bindingCode;
    int templateVersion = request->templateVersion;
    std::string printerName = request->printerName;
    int copies = request->copies;
    std::string operatorId = request->operatorId;
    std::string operatorName = request->operatorName;
    std::string clientType = request->clientType;
    std::string bizRefJson = request->bizRefJson;

    // 调用DAO插入打印日志
    int logResult = m_dao.insertPrintLog(
        traceId,
        bindingCode,
        templateVersion,
        printerName,
        copies,
        operatorId,
        operatorName,
        clientType,
        bizRefJson
    );


    return logResult;
}
