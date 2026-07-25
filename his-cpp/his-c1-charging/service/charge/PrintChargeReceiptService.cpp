#include "stdafx.h"
#include "PrintChargeReceiptService.h"
#include "dao/charge/PrintChargeReceiptDAO.h"
#include "dao/charge/GetChargeItemDAO.h"
#include "id/UuidFacade.h"

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
        slip->ageText = s.getAgeText();
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

        // 加载收费项明细
        GetChargeItemDAO ciDao;
        auto items = ciDao.selectByEncounterId(s.getEncounterId());
        auto lines = oatpp::Vector<oatpp::Object<ChargeReceiptChargeLineDTO>>::createShared();
        for (auto& item : items)
        {
            auto line = ChargeReceiptChargeLineDTO::createShared();
            line->itemName = item.getItemName();
            line->quantityText = item.getQuantityValue() + " " + item.getQuantityUnit();
            line->amountText = item.getTotalPrice();
            line->chargeCategoryText = item.getChargeCategoryCode();
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
        dto->scenarioCode ? dto->scenarioCode->c_str() : "",
        version, status,
        dto->bizRef ? "{}" : "{}",
        "{}", printer, copies, clientType, errorCode, errorMessage);

    resp->ok = (rows > 0);
    return resp;
}
