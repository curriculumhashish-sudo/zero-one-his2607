#include "stdafx.h"
#include "ChargeConfirmService.h"
#include "SimpleDateTimeFormat.h"
#include "../../dao/register/ChargeConfirmDAO.h" 
#include <chrono>
#include <random>

static std::string generateSimpleId() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return std::to_string(ms) + std::to_string(dis(gen));
}
bool ChargeConfirmService::confirmPayment(const ChargeConfirmDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 校验参数是否合法
	if (!dto->visitNumber || dto->visitNumber->empty())
	{
		return false;
	}
	if (!dto->amount || dto->amount <= 0)
	{
		return false;
	}
	if (!dto->paymentMethod || dto->paymentMethod < 1 || dto->paymentMethod > 4)
	{
		return false;
	}

	// 获取当前时间作为交易时间
	std::string transactionTime = SimpleDateTimeFormat::format();

	// 从 Token 获取操作员信息
	std::string operatorId = payload.getId();
	std::string operatorName = payload.getUsername();

	// 加载病人姓名和访问号等信息 
	std::string visitNumber = dto->visitNumber.getValue({}); //就诊号
	std::string patientName = dto->name.getValue({}); //病人姓名
	std::string phone = dto->phone.getValue({}); //手机号
	oatpp::Float64 amount = dto->amount;        //金额
	std::string paymentDraft = dto->paymentDraft.getValue({}); //支付草稿号
	std::string registrationType = dto->registrationType.getValue({}); //挂号类型
	oatpp::Int32 paymentMethod = dto->paymentMethod;  // 1-现金, 2-微信, 3-支付宝, 4-银行卡
  
    // 生成业务主键
    // reconciliationId 既是主键，也是支付流水号
    std::string reconciliationId = generateSimpleId();
    std::string invoiceId = generateSimpleId();
    std::string invoiceNo = "INV" + generateSimpleId();
    std::string detailId = generateSimpleId();
    // 定义空字符串变量，用于填补 DTO 中缺失的参数
    std::string emptyAccountId = "";
    std::string emptyChargeItemIds = ""; // DTO中没有收费项目ID列表，暂传空
    // 支付方式转换 (int 转 string)
    std::string paymentMethodStr = "CASH";
    if (paymentMethod == 2) paymentMethodStr = "WECHAT";
    else if (paymentMethod == 3) paymentMethodStr = "ALIPAY";
    else if (paymentMethod == 4) paymentMethodStr = "BANK";

    //执行业务逻辑
    ChargeConfirmDAO dao;
    // 更新支付对账单状态
    int version = 1;

    int updateRes = dao.updatePaymentReconciliation(
        paymentDraft,          // 1. 草稿ID
        paymentMethodStr,      // 2. 支付方式
        reconciliationId,      // 3. 支付流水号 (补上缺失参数)
        transactionTime,       // 4. 账单日期
        version                // 5. 版本号
    );

    if (updateRes <= 0) {
        return false; // 更新失败，返回错误
    }

    //插入支付明细
    dao.insertPaymentDetail(detailId, reconciliationId, emptyAccountId, paymentMethodStr, amount, amount, transactionTime, "SUCCESS", "PAYMENT");

    // 插入支付静态信息
    dao.insertPaymentStatic(reconciliationId, paymentMethodStr, paymentMethodStr, ZH_WORDS_GETTER("macchiato.charging.register.summary1"), amount, visitNumber);

    //  更新收费项目状态 (将对应的项目标记为已支付)

    int itemsUpdated = dao.updateChargeItemToPaid(paymentDraft);

    // 可以根据返回值判断是否有项目被更新，方便调试
    if (itemsUpdated <= 0) {
        return false; // 没有项目被更新，返回错误
    }

    //插入发票记录
    dao.insertInvoice(invoiceId, visitNumber, reconciliationId,detailId,visitNumber,invoiceNo,paymentMethodStr,amount,emptyChargeItemIds,transactionTime,paymentMethodStr);

    //更新就诊状态 (挂号确认)
    dao.updateEncounterToRegistered(visitNumber, version);

	return true;
}
