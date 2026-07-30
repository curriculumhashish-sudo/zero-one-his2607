#include "stdafx.h"
#include "PrintChargeReceiptService.h"
#include "dao/charge/PrintChargeReceiptDAO.h"
#include "dao/charge/GetChargeItemDAO.h"
#include "id/UuidFacade.h"

#include <set>
#include <sstream>

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

PrintChargeReceiptResponseDTO::Wrapper PrintChargeReceiptService::printChargeReceipt(const PrintChargeReceiptRequestDTO::Wrapper& dto)
{
    auto resp = PrintChargeReceiptResponseDTO::createShared();
    auto slip = SlipDTO::createShared();

    std::list<PrintChargeReceiptSlipDO> slipList;

    // 优先按 paymentId 查，其次按 encounterId
    if (dto->paymentId && !dto->paymentId->empty())
        slipList = PrintChargeReceiptDAO().selectSlipByPaymentId(dto->paymentId);
    else if (dto->encounterId && !dto->encounterId->empty())
        slipList = PrintChargeReceiptDAO().selectSlipByEncounterId(dto->encounterId);

    if (!slipList.empty())
    {
        auto& s = slipList.front();
        slip->encounterId = s.getEncounterId();
        slip->patientName = s.getPatientName();
        slip->hospitalName = s.getHospitalName();
        slip->genderEnumText = s.getGenderEnumText();
        // 根据出生日期计算年龄文本
        std::string birthDate = s.getBirthDate();
        if (!birthDate.empty() && birthDate.length() >= 4)
        {
            time_t now = time(nullptr);
            int currentYear = localtime(&now)->tm_year + 1900;
            int birthYear = std::stoi(birthDate.substr(0, 4));
            int age = currentYear - birthYear;
            if (age >= 0)
                slip->ageText = std::to_string(age) + ZH_WORDS_GETTER("lhfc.charging.charge.dict.ageSuffix");
        }
        slip->feeNatureText = s.getFeeNatureText();
        slip->totalAmountText = s.getTotalAmountText();
        slip->cashierName = s.getCashierName();
        slip->chargeTimeText = s.getChargeTimeText();

        // 费用分解（按渠道拆分，默认全部为现金）
        slip->cashPayText = s.getTotalAmountText();
        slip->wechatPayText = "";
        slip->alipayPayText = "";
        slip->poolingPayText = "";
        slip->fundPayText = "";
        slip->accountPayText = "";
        slip->deductibleLineText = "";
        slip->accountBalanceText = "";

        // 解析支付记录中的收费项ID列表
        std::set<std::string> paidIds;
        std::string idsStr = s.getChargeItemIds();
        if (!idsStr.empty())
        {
            std::istringstream iss(idsStr);
            std::string token;
            while (std::getline(iss, token, ','))
                if (!token.empty()) paidIds.insert(token);
        }

        // 加载收费项明细，仅保留本次支付的项
        GetChargeItemDAO ciDao;
        auto items = ciDao.selectByEncounterId(s.getEncounterId());
        auto lines = oatpp::Vector<oatpp::Object<ChargeReceiptChargeLineDTO>>::createShared();
        for (auto& item : items)
        {
            if (!paidIds.count(item.getChargeItemId()))
                continue;

            auto line = ChargeReceiptChargeLineDTO::createShared();
            // DTO字段, DO的Getter后缀
            ZO_STAR_DOMAIN_DO_TO_DTO(line, item,
                itemName, ItemName,
                amountText, TotalPrice,
                chargeCategoryText, ChargeCategoryCode
            );
            line->quantityText = item.getQuantityValue() + " " + item.getQuantityUnit();
            lines->push_back(line);
        }
        slip->chargeItemLines = lines;
    }

    resp->slip = slip;
    return resp;
}

PreparePrintResponseDTO::Wrapper PrintChargeReceiptService::preparePrint(const PreparePrintRequestDTO::Wrapper& dto)
{
    auto resp = PreparePrintResponseDTO::createShared();

    std::string code = dto->scenarioCode ? dto->scenarioCode->c_str() : "op-charge-print";
    auto templates = PrintChargeReceiptDAO().selectTemplateByScenario(code);

    if (!templates.empty())
    {
        auto& t = templates.front();
        resp->traceId = UuidFacade().genUuid();
        resp->busNo = t.getBusNo();
        resp->scenarioCode = t.getScenarioCode();
        try { resp->templateVersion = std::stoi(t.getTemplateVersion()); } catch (...) {}
    }

    return resp;
}

ConfirmPrintResponseDTO::Wrapper PrintChargeReceiptService::confirmPrint(const ConfirmPrintRequestDTO::Wrapper& dto)
{
    auto resp = ConfirmPrintResponseDTO::createShared();

    if (!dto->traceId || dto->traceId->empty())
    {
        resp->ok = false;
        return resp;
    }

    int version = dto->templateVersion ? dto->templateVersion : 0;
    std::string status = dto->status ? dto->status->c_str() : "SUCCESS";
    std::string printer = dto->printer ? dto->printer->c_str() : "";
    int copies = dto->copies ? dto->copies : 1;
    std::string clientType = dto->clientType ? dto->clientType->c_str() : "electron-hiprint";
    std::string errorCode = dto->errorCode ? dto->errorCode->c_str() : "";
    std::string errorMessage = dto->errorMessage ? dto->errorMessage->c_str() : "";

    int rows = PrintChargeReceiptDAO().insertPrintLog(
        dto->traceId,
        dto->busNo ? dto->busNo->c_str() : "",
        version, status,
        printer, copies, clientType, errorCode, errorMessage,
        dto->bizRef ? "{}" : "{}");
    // TODO: serialize dto->bizRef to JSON string

    resp->ok = (rows > 0);
    return resp;
}
