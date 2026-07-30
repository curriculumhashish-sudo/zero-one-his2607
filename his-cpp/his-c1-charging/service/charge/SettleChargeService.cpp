#include "stdafx.h"
#include "SettleChargeService.h"
#include "dao/charge/GetChargeItemDAO.h"
#include "dao/charge/SettleChargeDAO.h"
#include "id/UuidFacade.h"

#include <set>

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

PreSettleChargeResponseDTO::Wrapper SettleChargeService::preSettle(const PreSettleChargeRequestDTO::Wrapper& dto)
{
    auto resp = PreSettleChargeResponseDTO::createShared();

    if (!dto->encounterId || dto->encounterId->empty())
        return resp;

    // 构造选中ID集合，用于快速过滤
    std::set<std::string> selectedIds;
    if (dto->chargeItemIds)
    {
        for (auto& id : *dto->chargeItemIds)
            selectedIds.insert(std::string(id->c_str()));
    }

    // 读取就诊下所有收费项，仅保留用户选中的
    GetChargeItemDAO dao;
    auto items = dao.selectByEncounterId(dto->encounterId);

    // 计算展示应收金额并构建行预览
    double displayAmount = 0.0;
    auto previews = oatpp::Vector<oatpp::Object<PreSettleChargeLinePreviewDTO>>::createShared();

    for (auto& item : items)
    {
        // 跳过未被选中的收费项
        if (!selectedIds.empty() && !selectedIds.count(item.getChargeItemId()))
            continue;

        double linePrice = 0.0;
        try { linePrice = std::stod(item.getTotalPrice()); } catch (...) {}

        displayAmount += linePrice;

        auto preview = PreSettleChargeLinePreviewDTO::createShared();
        ZO_STAR_DOMAIN_DO_TO_DTO(preview, item,
            chargeItemId, ChargeItemId,
            itemLabel, ItemName,
            feeNatureDisplay,ContractName
        );
        preview->lineAmount = linePrice;
        previews->push_back(preview);
    }

    resp->displayAmount = displayAmount;
    resp->previews = previews;

    // 生成支付草稿ID
    resp->paymentDraftId = UuidFacade().genUuid();

    // 自费汇总
    auto selfPayRollup = OpSelfPayRollupDTO::createShared();
    selfPayRollup->totalAmount = displayAmount;
    if (dto->chargeItemIds)
        selfPayRollup->chargeItemIds = dto->chargeItemIds;
    resp->selfPayRollup = selfPayRollup;

    // 医保分组（需要对接医保平台2206接口，当前返回空）
    resp->insuranceGroups = oatpp::Vector<oatpp::Object<OpInsurancePreSettleGroupDTO>>::createShared();
    resp->settlementComposition = "SELF_PAY_ONLY";
    resp->insurancePreSettled = false;

    return resp;
}

ConfirmSettleChargeResponseDTO::Wrapper SettleChargeService::confirmSettle(const ConfirmSettleChargeRequestDTO::Wrapper& dto)
{
    auto resp = ConfirmSettleChargeResponseDTO::createShared();

    if (!dto->encounterId || dto->encounterId->empty())
        return resp;

    std::string paymentId = UuidFacade().genUuid();
    std::string invoiceId = UuidFacade().genUuid();
    std::string encounterId = dto->encounterId;

    // 计算总金额
    double totalAmount = 0.0;
    if (dto->paymentDetails)
    {
        for (auto& detail : *dto->paymentDetails)
            try { totalAmount += detail->amount; } catch (...) {}
    }
    std::string totalAmountStr = std::to_string(totalAmount);

    // 拼接 chargeItemIds
    std::string chargeItemIdsStr;
    if (dto->chargeItemIds)
    {
        for (size_t i = 0; i < dto->chargeItemIds->size(); i++)
        {
            if (i > 0) chargeItemIdsStr += ",";
            chargeItemIdsStr += (*dto->chargeItemIds)[i];
        }
    }

    // 当前时间
    time_t now = time(nullptr);
    char timeBuf[32];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    std::string nowStr(timeBuf);

    // 账单日期
    std::string billDate = (dto->billDate && !dto->billDate->empty())
        ? std::string(dto->billDate->c_str()) : nowStr;

    // 支付流水号
    std::string paymentNo = std::string("PMR-") + timeBuf;

    // 通过 DAO 执行写操作
    SettleChargeDAO dao;

    // 1. 插入支付流水
    dao.insertPayment(paymentId, nowStr, paymentNo, encounterId,
        totalAmountStr, chargeItemIdsStr, billDate, invoiceId);

    // 2. 插入发票草稿
    dao.insertInvoice(invoiceId, nowStr, encounterId, paymentId,
        chargeItemIdsStr, totalAmountStr);

    // 3. 批量更新收费项状态
    dao.updateChargeItemsToSettled(encounterId, nowStr);

    resp->paymentId = paymentId;
    resp->invoiceId = invoiceId;
    resp->invoiceStatus = "DRAFT";
    resp->successMessageCode = "EX-OPC-410";

    return resp;
}
