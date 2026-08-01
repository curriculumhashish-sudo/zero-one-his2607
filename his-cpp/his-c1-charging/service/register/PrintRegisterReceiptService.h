#pragma once
#ifndef _PRINT_REGISTER_RECEIPT_SERVICE_
#define _PRINT_REGISTER_RECEIPT_SERVICE_

#include "../../dao/register/PrintRegisterReceiptDAO.h"
#include "../../domain/dto/register/PrintRegisterReceiptDTO.h"

class PrintRegisterReceiptService {
private:
    PrintRegisterReceiptDAO m_dao; // 实例化 DAO

public:
    /**
    * 1. 准备打印挂号小票
    * 作用：根据挂号业务场景，获取对应的打印模板 JSON
    * 对应 DAO：queryPrintTemplate
    */
    std::string preparePrint(const std::string& scenarioCode);

    /**
     * 2. 确认打印挂号小票
     * 作用：记录打印日志，并更新挂号记录的打印次数
     * 对应 DAO：insertPrintLog, updateChargeItemPrintCount (或类似的更新方法)
     */
    int confirmPrint(const oatpp::Object<PrintRegisterReceiptDTO>& request);
};

#endif // !_PRINT_REGISTER_RECEIPT_SERVICE_
