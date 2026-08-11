#include "stdafx.h"
#include "SettleChargeService.h"
#include "dao/charge/GetChargeItemDAO.h"
#include "dao/charge/SettleChargeDAO.h"
#include "id/UuidFacade.h"

#include <chrono>
#include <cmath>
#include <set>
#include "SettleDraftStore.h"

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

    // 生成支付草稿ID并存入快照，供3B校验防换单
    std::string draftId = UuidFacade().genUuid();
    resp->paymentDraftId = draftId;

    SettleDraftSnapshot snapshot;
    snapshot.encounterId = dto->encounterId.getValue("");
    snapshot.totalAmount = displayAmount;
    if (dto->chargeItemIds)
        for (auto& id : *dto->chargeItemIds)
            snapshot.chargeItemIds.push_back(std::string(id->c_str()));
    SettleDraftStore::getInstance().put(draftId, snapshot);

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

ConfirmSettleChargeResponseDTO::Wrapper SettleChargeService::confirmSettle(
    const ConfirmSettleChargeRequestDTO::Wrapper& dto)
{
    auto resp = ConfirmSettleChargeResponseDTO::createShared();

    if (!dto->encounterId || dto->encounterId->empty())
        return resp;

    // 校验支付草稿ID，防重放和防换单
    std::string draftId = dto->paymentDraftId ? std::string(dto->paymentDraftId->c_str()) : "";
    SettleDraftSnapshot draft;
    if (draftId.empty() || !SettleDraftStore::getInstance().take(draftId, draft))
    {
        resp->successMessageCode = "EX-DRAFT-404";
        resp->paymentId = "";
        resp->invoiceId = "";
        resp->invoiceStatus = "";
        return resp;
    }
    if (draft.encounterId != std::string(dto->encounterId->c_str()))
    {
        resp->successMessageCode = "EX-DRAFT-ENCOUNTER_MISMATCH";
        resp->paymentId = "";
        resp->invoiceId = "";
        resp->invoiceStatus = "";
        return resp;
    }
    // 比对选中收费项集合，防换单
    std::set<std::string> draftIds(draft.chargeItemIds.begin(), draft.chargeItemIds.end());
    std::set<std::string> reqIds;
    if (dto->chargeItemIds)
        for (auto& id : *dto->chargeItemIds) reqIds.insert(std::string(id->c_str()));
    if (draftIds != reqIds)
    {
        resp->successMessageCode = "EX-DRAFT-ITEMS_MISMATCH";
        resp->paymentId = "";
        resp->invoiceId = "";
        resp->invoiceStatus = "";
        return resp;
    }

    std::string paymentId = UuidFacade().genUuid();
    std::string invoiceId = UuidFacade().genUuid();
    std::string encounterId = dto->encounterId;

    // 从DTO中获取Controller注入的操作人载荷
    std::string entererId = dto->getPayload() ? dto->getPayload()->getId() : "";
    // 根据就诊记录查询真实 patientId
    SettleChargeDAO dao;
    std::string patientId = dao.selectPatientIdByEncounter(encounterId);

    // 计算总金额
    double totalAmount = 0.0;
    if (dto->paymentDetails)
    {
        for (auto& detail : *dto->paymentDetails)
            try { totalAmount += detail->amount; } catch (...) {}
    }
    std::string totalAmountStr = std::to_string(totalAmount);

    // 校验实付合计与预结算草稿金额一致
    if (std::abs(totalAmount - draft.totalAmount) > 0.001)
    {
        resp->successMessageCode = "EX-DRAFT-AMOUNT_MISMATCH";
        resp->paymentId = "";
        resp->invoiceId = "";
        resp->invoiceStatus = "";
        return resp;
    }

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
    char nowBuf[32];
    strftime(nowBuf, sizeof(nowBuf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    std::string nowStr(nowBuf);

    // 账单日期
    std::string billDate = (dto->billDate && !dto->billDate->empty())
        ? std::string(dto->billDate->c_str()) : nowStr;

    // 日期部分（用于生成流水号）
    char dateBuf[16];
    strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", localtime(&now));
    // 毫秒后4位作为序列号
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() % 10000;
    char seqBuf[8];
    snprintf(seqBuf, sizeof(seqBuf), "%04lld", (long long)ms);

    // 支付流水号 PMR-YYYY-MM-DDNNNN
    std::string paymentNo = std::string("PMR-") + dateBuf + seqBuf;
    // 发票业务号 IVYYYY-MM-DDNNNN
    std::string invoiceBusNo = std::string("IV") + dateBuf + seqBuf;

    // ---- 事务写入 ----
    dao.beginTransaction();

    try
    {
        // 1. 插入支付流水
        if (dao.insertPayment(paymentId, nowStr, paymentNo, entererId,
                patientId, totalAmountStr, chargeItemIdsStr, billDate, invoiceId,
                encounterId) < 1)
        {
            dao.rollbackTransaction();
            resp->successMessageCode = "EX-INSERT-PAYMENT-FAILED";
            resp->paymentId = "";
            resp->invoiceId = "";
            resp->invoiceStatus = "";
            return resp;
        }

        // 2. 逐条保存支付明细到 fin_payment_rec_detail
        if (dto->paymentDetails)
        {
            for (auto& detail : *dto->paymentDetails)
            {
                std::string detailId = UuidFacade().genUuid();
                std::string channelCode = detail->payChannelCode
                    ? std::string(detail->payChannelCode->c_str()) : "CASH";
                std::string amountStr = std::to_string(detail->amount.getValue(0.0));
                if (dao.insertPaymentDetail(detailId, nowStr, paymentId,
                        channelCode, amountStr, entererId) < 1)
                {
                    dao.rollbackTransaction();
                    resp->successMessageCode = "EX-INSERT-PAYMENT-DETAIL-FAILED";
                    resp->paymentId = "";
                    resp->invoiceId = "";
                    resp->invoiceStatus = "";
                    return resp;
                }
            }
        }

        // 3. 插入发票草稿
        if (dao.insertInvoice(invoiceId, nowStr, patientId, paymentId,
                chargeItemIdsStr, totalAmountStr, invoiceBusNo) < 1)
        {
            dao.rollbackTransaction();
            resp->successMessageCode = "EX-INSERT-INVOICE-FAILED";
            resp->paymentId = "";
            resp->invoiceId = "";
            resp->invoiceStatus = "";
            return resp;
        }

        // 4. 更新选中的收费项状态为已结算
        int expectedRows = static_cast<int>(draft.chargeItemIds.size());
        if (dao.updateChargeItemsToSettled(chargeItemIdsStr, nowStr) < expectedRows)
        {
            dao.rollbackTransaction();
            resp->successMessageCode = "EX-UPDATE-CHARGE-ITEMS-FAILED";
            resp->paymentId = "";
            resp->invoiceId = "";
            resp->invoiceStatus = "";
            return resp;
        }

        dao.commitTransaction();
    }
    catch (...)
    {
        dao.rollbackTransaction();
        resp->successMessageCode = "EX-SETTLE-TRANSACTION-FAILED";
        resp->paymentId = "";
        resp->invoiceId = "";
        resp->invoiceStatus = "";
        return resp;
    }

    resp->paymentId = paymentId;
    resp->invoiceId = invoiceId;
    resp->invoiceStatus = "DRAFT";
    resp->paymentNo = paymentNo;
    resp->statusEnum = "SUCCESS";
    resp->settledAt = nowStr;
    resp->successMessageCode = "EX-OPC-410";

    return resp;
}
