#include "stdafx.h"
#include "OrderStopService.h"
#include "dao/medicalorder/MedOrderDAO.h"
#include "dao/medicalorder/WkfServiceRequestDAO.h"
#include "dao/medicalorder/DispenseDAO.h"
#include "dao/medicalorder/ChargeItemDAO.h"
#include "dao/medicalorder/AdmEncounterDAO.h"
#include <algorithm>


//产品表路由
bool OrderStopService::isMedicationTable(const std::string& productTableName)
{
    std::string pt = normalizeTableName(productTableName);
    return pt == "med_medication_definition";
}

bool OrderStopService::isActivityTable(const std::string& productTableName)
{
    std::string pt = normalizeTableName(productTableName);
    return pt == "wkf_activity_definition";
}

std::string OrderStopService::normalizeTableName(const std::string& raw)
{
    std::string result = raw;
    // trim
    size_t start = result.find_first_not_of(" \t\n\r");
    size_t end = result.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    result = result.substr(start, end - start + 1);
    // tolower
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}


//gateValidateEncounter — 统一门控
bool OrderStopService::gateValidateEncounter(
    const std::string& encounterId,
    const std::string& patientId,
    bool writable)
{
    AdmEncounterDAO dao;
    auto enc = dao.selectById(encounterId);
    if (!enc) return false;
    if (enc->getPatientId() != patientId) return false;
    if (writable) {
        std::string s = enc->getStatusEnum();
        if (s != "IN_PROGRESS" && s != "ON_HOLD") return false;
    }
    return true;
}

// Pre-condition assertions
void OrderStopService::assertLongTermMedication(const std::string& requestId)
{
    MedOrderDAO dao;
    auto order = dao.selectById(requestId);
    if (!order) {
        throw std::runtime_error("Medication order not found: " + requestId);
    }

    if (order->getTherapyEnum() != "LONG_TERM") {
        throw std::runtime_error(
            "Only long-term medication orders can be stopped, id="
            + requestId);
    }
}

void OrderStopService::assertLongTermService(
    const std::string& encounterId, const std::string& requestId)
{
    WkfServiceRequestDAO dao;
    auto order = dao.selectById(requestId);
    if (!order) {
        throw std::runtime_error("Service request not found: " + requestId);
    }
    if (order->getTherapyEnum() != "LONG_TERM") {
        throw std::runtime_error(
            "Only long-term service requests can be stopped, id=" + requestId);
    }
}

//stopOrders—IF-IP-DOR-20 完整实现（批量版）
StopResultDTO::Wrapper OrderStopService::stopOrders(
    const OrderStopDTO::Wrapper& dto)
{
    // ---- Gate 1: 就诊快照 ----
    std::string encId = dto->encounterId.getValue("");
    std::string patId = dto->patientId.getValue("");
    if (encId.empty() || patId.empty()) {
        return nullptr;
    }
    if (!gateValidateEncounter(encId, patId, true)) {
        return nullptr;
    }

    // ---- Gate 2: 操作人 ----
    std::string operatorId = dto->operatorId.getValue("");
    if (operatorId.empty()) {
        return nullptr;
    }

    // ---- Gate 3: lines 非空
    if (!dto->lines || dto->lines->size() == 0) {
        auto result = StopResultDTO::createShared();
        result->successCount = 0;
        result->failCount = 0;
        return result;
    }

    // ---- 准备 ----
    MedOrderDAO medDAO;
    WkfServiceRequestDAO srvDAO;
    DispenseDAO dispDAO;
    ChargeItemDAO chargeDAO;
    int stopReason = dto->stopReason ? dto->stopReason.getValue(0) : 0;

    std::list<std::string> medicationIds;  // 收集药品类 ID
    std::list<std::string> activityIds;    // 收集诊疗类 ID
    int failCount = 0;
    std::list<std::string> failReasons;

    // ---- 逐行前置校验 + 按表分类 ----
    for (auto& line : *dto->lines) {
        std::string requestId = line->requestId.getValue("");
        std::string pt = normalizeTableName(
            line->productTableName.getValue(""));

        if (requestId.empty()) {
            failCount++;
            failReasons.push_back("停嘱行 requestId 为空");
            continue;
        }

        // 药品路径：前置校验通过后收集到 medicationIds
        if (isMedicationTable(pt)) {
            try {
                // Pre 1: 必须是长期医嘱
                assertLongTermMedication(requestId);

                // Pre 2: 发药检查
                auto dispRecord = dispDAO.selectByMedReqId(requestId);
                if (dispRecord) {
                    std::string dispStatus = dispRecord->getStatusEnum();
                    if (dispStatus == "dispensed") {
                        failCount++;
                        failReasons.push_back(
                            "医嘱 " + requestId + " 已发药，需先退药");
                        continue;
                    }
                }

                // Pre 3: 收费检查
                auto chargeItems = chargeDAO.selectByService(
                    "med_medication_request", requestId);
                bool hasCharged = false;
                for (auto& ci : chargeItems) {
                    if (ci->getStatusEnum() == "charged") {
                        hasCharged = true;
                        break;
                    }
                }
                if (hasCharged) {
                    failCount++;
                    failReasons.push_back(
                        "医嘱 " + requestId + " 已收费，需先退费");
                    continue;
                }

                // 通过所有校验，加入批量列表
                medicationIds.push_back(requestId);
            }
            catch (const std::exception& e) {
                failCount++;
                failReasons.push_back(
                    std::string("药品停嘱异常: ") + e.what());
            }
            continue;
        }

        // 诊疗路径：前置校验通过后收集到 activityIds
        if (isActivityTable(pt)) {
            try {
                // Pre: 必须是长期诊疗
                assertLongTermService(encId, requestId);
                // 通过校验，加入批量列表
                activityIds.push_back(requestId);
            }
            catch (const std::exception& e) {
                failCount++;
                failReasons.push_back(
                    std::string("诊疗停嘱异常: ") + e.what());
            }
            continue;
        }

        // 不支持的产品表
        failCount++;
        failReasons.push_back(
            "不支持的产品表名: " + pt + " (id=" + requestId + ")");
    }

    // ---- 批量执行 ----
    int successCount = 0;

    // 药品批量停嘱
    if (!medicationIds.empty()) {
        int updated = medDAO.stopOrderBatch(
            medicationIds, stopReason, operatorId);
        successCount += updated;
        int skipped = static_cast<int>(medicationIds.size()) - updated;
        if (skipped > 0) {
            failCount += skipped;
            failReasons.push_back(
                std::to_string(skipped) + " 条药品医嘱停嘱失败(可能已停嘱)");
        }
    }

    // 诊疗批量停嘱
    if (!activityIds.empty()) {
        int updated = srvDAO.stopOrderBatch(
            activityIds, stopReason, operatorId);
        successCount += updated;
        int skipped = static_cast<int>(activityIds.size()) - updated;
        if (skipped > 0) {
            failCount += skipped;
            failReasons.push_back(
                std::to_string(skipped) + " 条诊疗医嘱停嘱失败(可能已停嘱)");
        }
    }

    // ---- 返回结果 ----
    auto result = StopResultDTO::createShared();
    result->successCount = successCount;
    result->failCount = failCount;
    if (!failReasons.empty()) {
        auto list = oatpp::List<oatpp::String>::createShared();
        for (auto& r : failReasons) {
            list->push_back(r);
        }
        result->failReasons = list;
    }
    return result;
}