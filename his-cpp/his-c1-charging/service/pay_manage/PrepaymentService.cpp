#include "stdafx.h"
#include "PrepaymentService.h"
#include "../../dao/pay_manage/PrepaymentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

PrepaymentAddResponseDTO::Wrapper PrepaymentService::addPrepayment(const PrepaymentAddDTO::Wrapper& dto)
{
    auto response = PrepaymentAddResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    double amount = dto->amount.getValue(0.0);
    std::string paymentMethod = dto->paymentMethod.getValue("1");
    std::string operatorName = dto->operatorName.getValue("");
    std::string remark = dto->remark.getValue("");

    UuidFacade uf;
    std::string uuid = uf.genUuid();
    std::string now = SimpleDateTimeFormat::format();

    // 获取最新账户快照用于余额链接
    PrepaymentDAO dao;
    auto snap = dao.getAccountSnapshot(inpatientNo);
    double afterBalance = snap.balance + amount;

    // 构建支付说明：支付方式 + 可选备注
    std::string payText = paymentMethod;
    if (!remark.empty()) payText += "," + remark;

    // 通过原生SQL插入（charge模块风格）
    dao.insertPrepayment(
        uuid, now, "01xq", operatorName,
        snap.accountId, inpatientNo,
        "CHARGE", amount, amount, 0.0,
        uuid, payText, now,
        snap.balance, afterBalance,
        snap.predecessorId);

    // 构建响应
    response->prepaymentId = uuid;
    response->inpatientNo = inpatientNo;
    response->amount = amount;
    response->paymentMethod = paymentMethod;
    response->createTime = now;

    return response;
}

PrepaymentRefundResponseDTO::Wrapper PrepaymentService::refundPrepayment(const PrepaymentRefundDTO::Wrapper& dto)
{
    auto response = PrepaymentRefundResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    double amount = dto->amount.getValue(0.0);
    std::string refundMethod = dto->refundMethod.getValue("1");
    std::string operatorName = dto->operatorName.getValue("");
    std::string reason = dto->reason.getValue("");
    std::string remark = dto->remark.getValue("");

    UuidFacade uf;
    std::string uuid = uf.genUuid();
    std::string now = SimpleDateTimeFormat::format();

    // 获取最新账户快照用于余额链接
    PrepaymentDAO dao;
    auto snap = dao.getAccountSnapshot(inpatientNo);

    // 检查余额是否充足
    if (amount > snap.balance)
    {
        throw std::runtime_error("insufficient balance: available="
            + std::to_string(snap.balance) + ", refund=" + std::to_string(amount));
    }
    double afterBalance = snap.balance - amount;

    // 构建支付说明：退款方式 + 退款原因 + 备注
    std::string payText = refundMethod;
    if (!reason.empty()) payText += "," + reason;
    if (!remark.empty()) payText += "," + remark;

    // 通过原生SQL插入（charge模块风格）
    dao.insertPrepayment(
        uuid, now, "01xq", operatorName,
        snap.accountId, inpatientNo,
        "REFUND", amount, 0.0, amount,
        uuid, payText, now,
        snap.balance, afterBalance,
        snap.predecessorId);

    // 构建响应
    response->prepaymentId = uuid;
    response->inpatientNo = inpatientNo;
    response->amount = amount;
    response->refundMethod = refundMethod;
    response->createTime = now;

    return response;
}

PrepaymentPageDTO::Wrapper PrepaymentService::queryPrepayment(const PrepaymentQuery::Wrapper& query)
{
    auto page = PrepaymentPageDTO::createShared();
    PrepaymentDAO dao;

    // 统计并查询
    uint64_t total = dao.count(query);
    auto doList = dao.selectWithPage(query);

    // DO映射到DTO
    auto rows = oatpp::List<PrepaymentRecordDTO::Wrapper>::createShared();
    for (auto& record : doList)
    {
        auto dto = PrepaymentRecordDTO::createShared();
        dto->prepaymentId = record.getId();
        dto->inpatientNo = record.getAccountCode();
        dto->patientName = record.getAccountCode();  // TODO: 关联患者表获取真实姓名
        dto->amount = record.getAmount();
        dto->prepayType = record.getPayEnum();
        dto->method = record.getPayTransText();
        dto->operatorName = record.getCreatedBy();
        dto->createTime = record.getPayTransDate();
        rows->push_back(dto);
    }

    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);
    page->total = v_int64(total);
    page->calcPages();
    page->rows = rows;

    return page;
}
