#include "stdafx.h"
#include "SettlementService.h"
#include "dao/charge/ChargeItemDAO.h"
#include "id/UuidFacade.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

PreSettleChargeResponseDTO::Wrapper SettlementService::preSettle(const PreSettleChargeRequestDTO::Wrapper& dto)
{
    auto resp = PreSettleChargeResponseDTO::createShared();

    if (!dto->encounterId || dto->encounterId->empty())
        return resp;

    // 读取收费项明细
    ChargeItemDAO dao;
    auto items = dao.selectByEncounterId(dto->encounterId);

    // 计算展示应收金额并构建行预览
    double displayAmount = 0.0;
    auto previews = oatpp::Vector<oatpp::Object<PreSettleChargeLinePreviewDTO>>::createShared();

    for (auto& item : items)
    {
        double linePrice = 0.0;
        try { linePrice = std::stod(item.getTotalPrice()); } catch (...) {}

        displayAmount += linePrice;

        auto preview = PreSettleChargeLinePreviewDTO::createShared();
        preview->chargeItemId = item.getChargeItemId();
        preview->itemLabel = item.getItemName();
        preview->lineAmount = linePrice;
        preview->feeNatureDisplay = item.getContractName();
        previews->push_back(preview);
    }

    resp->displayAmount = displayAmount;
    resp->previews = previews;

    // 生成支付草稿ID
    resp->paymentDraftId = UuidFacade::generateUuid();

    // 自费汇总
    auto selfPayRollup = OpSelfPayRollupDTO::createShared();
    selfPayRollup->totalAmount = displayAmount;
    resp->selfPayRollup = selfPayRollup;

    // 医保分组（需要对接医保平台2206接口，当前返回空）
    resp->insuranceGroups = oatpp::Vector<oatpp::Object<OpInsurancePreSettleGroupDTO>>::createShared();
    resp->settlementComposition = "SELF_PAY_ONLY";
    resp->insurancePreSettled = false;

    return resp;
}

ConfirmSettleChargeResponseDTO::Wrapper SettlementService::confirmSettle(const ConfirmSettleChargeRequestDTO::Wrapper& dto)
{
    auto resp = ConfirmSettleChargeResponseDTO::createShared();

    if (!dto->encounterId || dto->encounterId->empty())
        return resp;

    std::string paymentId = UuidFacade::generateUuid();
    std::string invoiceId = UuidFacade::generateUuid();

    // 计算总金额
    double totalAmount = 0.0;
    if (dto->paymentDetails)
    {
        for (auto& detail : *dto->paymentDetails)
            try { totalAmount += detail->amount; } catch (...) {}
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
    char timeBuf[32];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    std::string nowStr(timeBuf);

    // 支付流水号
    std::string paymentNo = std::string("PMR-") + timeBuf;

    // INSERT fin_payment_reconciliation
    {
        SqlParams params;
        std::ostringstream sql;
        sql << "INSERT INTO fin_payment_reconciliation"
            << " (id, tenant_id, created_at, is_deleted, version,"
            << " status_enum, payment_no, payment_enum, kind_enum,"
            << " enterer_id, patient_id, tendered_amount, display_amount,"
            << " print_count, charge_item_ids, encounter_id, bill_date, invoice_id)"
            << " VALUES (?,NULL,?,0,0, 'SUCCESS',?,?,?, ?,?,?,?, 0,?,?,?,?)";

        SQLPARAMS_PUSH(params, "s", std::string, paymentId);
        SQLPARAMS_PUSH(params, "s", std::string, nowStr);
        SQLPARAMS_PUSH(params, "s", std::string, paymentNo);
        SQLPARAMS_PUSH(params, "s", std::string, "CASH_PAY");
        SQLPARAMS_PUSH(params, "s", std::string, "SETTLEMENT");
        SQLPARAMS_PUSH(params, "s", std::string, dto->encounterId);
        SQLPARAMS_PUSH(params, "s", std::string, dto->encounterId);
        SQLPARAMS_PUSH(params, "s", std::string, std::to_string(totalAmount));
        SQLPARAMS_PUSH(params, "s", std::string, std::to_string(totalAmount));
        SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);
        SQLPARAMS_PUSH(params, "s", std::string, dto->encounterId);
        SQLPARAMS_PUSH(params, "s", std::string, dto->billDate ? dto->billDate->getValue("") : nowStr);
        SQLPARAMS_PUSH(params, "s", std::string, invoiceId);

        sqlSession->executeUpdate(sql.str(), params);
    }

    // INSERT adm_invoice (DRAFT)
    {
        SqlParams params;
        std::ostringstream sql;
        sql << "INSERT INTO adm_invoice"
            << " (id, tenant_id, created_at, is_deleted, version,"
            << " patient_id, reconciliation_id, payment_id,"
            << " charge_item_ids, amount, status_enum)"
            << " VALUES (?,NULL,?,0,0, ?,?,?, ?,?,'DRAFT')";

        SQLPARAMS_PUSH(params, "s", std::string, invoiceId);
        SQLPARAMS_PUSH(params, "s", std::string, nowStr);
        SQLPARAMS_PUSH(params, "s", std::string, dto->encounterId);
        SQLPARAMS_PUSH(params, "s", std::string, paymentId);
        SQLPARAMS_PUSH(params, "s", std::string, paymentId);
        SQLPARAMS_PUSH(params, "s", std::string, chargeItemIdsStr);
        SQLPARAMS_PUSH(params, "s", std::string, std::to_string(totalAmount));

        sqlSession->executeUpdate(sql.str(), params);
    }

    // UPDATE adm_charge_item -> SETTLED
    if (dto->chargeItemIds)
    {
        for (auto& ciId : *dto->chargeItemIds)
        {
            SqlParams params;
            SQLPARAMS_PUSH(params, "s", std::string, nowStr);
            SQLPARAMS_PUSH(params, "s", std::string, ciId);
            sqlSession->executeUpdate(
                "UPDATE adm_charge_item SET status_enum='SETTLED', updated_at=? WHERE id=?", params);
        }
    }

    resp->paymentId = paymentId;
    resp->invoiceId = invoiceId;
    resp->invoiceStatus = "DRAFT";
    resp->successMessageCode = "EX-OPC-410";

    return resp;
}
