#include "QuitNumberService.h"
#include "../../dao/register/QuitNumberDAO.h"
#include <string>
static std::string generateSimpleId() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return std::to_string(ms) + std::to_string(dis(gen));
}

bool QuitNumberService::confirmQuitNumber(const oatpp::Object<QuitNumberDTO>& request, const PayloadDTO& payload) {
    // 1. 参数校验（基于现有DTO字段）
    if (!request->visitNumber || request->visitNumber->empty()) {
        return false; // 就诊号不能为空
    }
    if (!request->refundAmount || request->refundAmount <= 0) {
        return false; // 退款金额必须大于0
    }

    // 2. 从payload获取操作员信息
    std::string entererId = payload.getId();
    std::string entererName = payload.getUsername();

    // 3. 从DTO中提取核心字段（直接使用现有字段）
    std::string encounterId = request->visitNumber.getValue({}); // 就诊号作为就诊记录ID
    oatpp::Float64 refundAmount = request->refundAmount;        // 应退金额
    std::string status = request->status.getValue({});         // 当前状态（如"REGISTERED"）
    int version = 1;                 // 版本号（乐观锁）
    // 4. 生成业务主键（退款唯一标识）
    std::string refundId = generateSimpleId(); // 假设generateSimpleId是生成唯一ID的函数

    // 5. 执行DAO操作（按业务顺序执行，失败则返回false）
    QuitNumberDAO dao;

    // 步骤1：更新就诊记录状态（REGISTERED → CANCELLED）
    int result1 = dao.updateEncounterToCancelled(encounterId, version);
    if (result1 <= 0) return false;

    // 步骤2：标记候诊队列为取消
    int result2 = dao.updateQueueToCancelled(encounterId, ZH_WORDS_GETTER("macchiato.charging.register.summary2"));
    if (result2 <= 0) return false;

    // 步骤3：释放号源
    int result3 = dao.releaseSlot(request->slotId);
    if (result3 <= 0) return false;

    // 步骤4：更新收费项目状态
    int result4 = dao.updateChargeItemToRefunded(request->chargeItemId, request->refundId, version);
    if (result4 <= 0) return false;

    // 步骤5：插入退款支付对账记录
     int result5 = dao.insertRefundPayment(refundId, request->patientId, encounterId, request->paymentNo, request->payEnum, refundAmount, request->chargeItemIds, entererId);
    if (result5 <= 0) return false;

    // 步骤6：插入退款支付明细
    int result6 = dao.insertRefundDetail(request->detailId, refundId, request->payEnum, refundAmount);
    if (result6 <= 0) return false;

    // 步骤7：更新发票状态
    int result7 = dao.updateInvoiceToCancelled(request->invoiceId, ZH_WORDS_GETTER("macchiato.charging.register.summary2"));
    if (result7 <= 0) return false;

    // 步骤8：退还账户余额
    int result8 = dao.updateAccountBalanceRefund(request->accountId, refundAmount);
    if (result8 <= 0) return false;

    return true; // 全部核心步骤成功
}
