#pragma once
#ifndef _PRINT_REGISTER_RECEIPT_DAO_
#define _PRINT_REGISTER_RECEIPT_DAO_

#include "BaseDAO.h"
#include <string>

class PrintRegisterReceiptDAO : public BaseDAO {
public:
    // 查询打印绑定
    std::string queryPrintBinding(const std::string& busNo);

    // 查询打印模板
    std::string queryPrintTemplate(const std::string& scenarioCode, const std::string& status);

    // 插入打印日志
    int insertPrintLog(const std::string& traceId, const std::string& bindingCode, int templateVersion,
        const std::string& printerName, int copies, const std::string& operatorId,
        const std::string& operatorName, const std::string& clientType, const std::string& bizRefJson);

};

#endif // !_PRINT_REGISTER_RECEIPT_DAO_
