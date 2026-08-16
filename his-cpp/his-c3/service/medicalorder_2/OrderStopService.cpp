#include "stdafx.h"
#pragma execution_character_set("utf-8")
#include "OrderStopService.h"
#include "dao/medicalorder_2/MedOrderDAO.h"
#include "dao/medicalorder_2/WkfServiceRequestDAO.h"
#include "dao/medicalorder_2/DispenseDAO.h"
#include "dao/medicalorder_2/ChargeItemDAO.h"
#include "dao/medicalorder_2/AdmEncounterDAO.h"
#include <algorithm>


// Product table name routing
bool OrderStopService::isMedicationTable(const std::string& productTableName)
{
    std::string pt = normalizeTableName(productTableName);
    return pt == "med_medication_request";
}

bool OrderStopService::isActivityTable(const std::string& productTableName)
{
    std::string pt = normalizeTableName(productTableName);
    return pt == "wkf_service_request";
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


// Unified gate control for encounter validation
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
void OrderStopService::assertLongTermMedication(const std::string& requestId, const std::string& encounterId, const std::string& patientId)
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
    if (order->getEncounterId() != encounterId || order->getPatientId() != patientId) {
        throw std::runtime_error("Error of encounterId or patientId, id = " + requestId);
    }
}

void OrderStopService::assertLongTermService(
    const std::string& encounterId, const std::string& requestId, const std::string& patientId)
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
    if (order->getEncounterId() != encounterId || order->getPatientId() != patientId) {
        throw std::runtime_error("Error of encounterId or patientId, id = " + requestId);
    }
}

// Batch stop orders
StopResultDTO::Wrapper OrderStopService::stopOrders(
    const OrderStopDTO::Wrapper& dto)
{
    // ---- Gate 1: Encounter validation ----
    std::string encId = dto->encounterId.getValue("");
    std::string patId = dto->patientId.getValue("");
    if (encId.empty() || patId.empty()) {
        auto result = StopResultDTO::createShared();
        result->successCount = 0;
        result->failCount = 1;
        auto details = oatpp::List<oatpp::Object<StopFailDetailDTO>>::createShared();
        auto detail = StopFailDetailDTO::createShared();
        detail->requestId = "";
        detail->reason = "Missing encounter ID or patient ID";
        details->push_back(detail);
        result->failDetails = details;
        return result;
    }
    if (!gateValidateEncounter(encId, patId, true)) {
        auto result = StopResultDTO::createShared();
        result->successCount = 0;
        result->failCount = 1;
        auto details = oatpp::List<oatpp::Object<StopFailDetailDTO>>::createShared();
        auto detail = StopFailDetailDTO::createShared();
        detail->requestId = "";
        detail->reason = "Encounter validation failed";
        details->push_back(detail);
        result->failDetails = details;
        return result;
    }

    // ---- Gate 2: Operator validation ----
    std::string operatorId = dto->operatorId.getValue("");
    if (operatorId.empty()) {
        auto result = StopResultDTO::createShared();
        result->successCount = 0;
        result->failCount = 1;
        auto details = oatpp::List<oatpp::Object<StopFailDetailDTO>>::createShared();
        auto detail = StopFailDetailDTO::createShared();
        detail->requestId = "";
        detail->reason = "Missing operator ID";
        details->push_back(detail);
        result->failDetails = details;
        return result;
    }

    // ---- Gate 3: lines non-empty check ----
    if (!dto->lines || dto->lines->size() == 0) {
        auto result = StopResultDTO::createShared();
        result->successCount = 0;
        result->failCount = 0;
        return result;
    }

    // ---- Preparation ----
    MedOrderDAO medDAO;
    WkfServiceRequestDAO srvDAO;
    DispenseDAO dispDAO;
    ChargeItemDAO chargeDAO;
    int stopReason = dto->stopReason ? dto->stopReason.getValue(0) : 0;

    std::list<std::string> medicationIds;
    std::list<std::string> activityIds;
    int failCount = 0;
    auto failDetails = oatpp::List<oatpp::Object<StopFailDetailDTO>>::createShared();

    // ---- Per-line pre-check + classification ----
    for (auto& line : *dto->lines) {
        std::string requestId = line->requestId.getValue("");
        std::string pt = normalizeTableName(
            line->productTableName.getValue(""));

        if (requestId.empty()) {
            failCount++;
            auto d = StopFailDetailDTO::createShared();
            d->requestId = "";
            d->reason = "Stop order target requestId is empty";
            failDetails->push_back(d);
            continue;
        }

        // Medication path: collect to medicationIds after pre-checks pass
        if (isMedicationTable(pt)) {
            try {
                assertLongTermMedication(requestId, encId, patId);

                // Dispense check
                auto dispRecord = dispDAO.selectByMedReqId(requestId);
                if (dispRecord) {
                    std::string dispStatus = dispRecord->getStatusEnum();
                    if (dispStatus == "dispensed") {
                        failCount++;
                        auto d = StopFailDetailDTO::createShared();
                        d->requestId = requestId;
                        d->reason = "Already dispensed, cannot stop";
                        failDetails->push_back(d);
                        continue;
                    }
                }

                // Charge check
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
                    auto d = StopFailDetailDTO::createShared();
                    d->requestId = requestId;
                    d->reason = "Already charged, cannot stop";
                    failDetails->push_back(d);
                    continue;
                }

                medicationIds.push_back(requestId);
            }
            catch (const std::exception& e) {
                failCount++;
                auto d = StopFailDetailDTO::createShared();
                d->requestId = requestId;
                d->reason = std::string("Medication stop exception: ") + e.what();
                failDetails->push_back(d);
            }
            continue;
        }

        // Service path: collect to activityIds after pre-checks pass
        if (isActivityTable(pt)) {
            try {
                assertLongTermService(encId, requestId, patId);
                activityIds.push_back(requestId);
            }
            catch (const std::exception& e) {
                failCount++;
                auto d = StopFailDetailDTO::createShared();
                d->requestId = requestId;
                d->reason = std::string("Service stop exception: ") + e.what();
                failDetails->push_back(d);
            }
            continue;
        }

        // Unsupported product type
        failCount++;
        auto d = StopFailDetailDTO::createShared();
        d->requestId = requestId;
        d->reason = "Unsupported product type: " + pt;
        failDetails->push_back(d);
    }

    // ---- Batch execution ----
    int successCount = 0;

    // Medication batch stop (transaction protected)
    if (!medicationIds.empty()) {
        try {
            medDAO.beginTransaction();
            int updated = medDAO.stopOrderBatch(medicationIds, stopReason, operatorId, encId, patId);
            medDAO.commitTransaction();
            successCount += updated;
            int skipped = static_cast<int>(medicationIds.size()) - updated;
            if (skipped > 0) {
                failCount += skipped;
                auto d = StopFailDetailDTO::createShared();
                d->requestId = "batch";
                d->reason = std::to_string(skipped) + " medication order(s) failed to stop (may already be stopped)";
                failDetails->push_back(d);
            }
        } catch (...) {
            medDAO.rollbackTransaction();
            failCount += static_cast<int>(medicationIds.size());
            auto d = StopFailDetailDTO::createShared();
            d->requestId = "batch";
            d->reason = "Medication order batch stop exception, rolled back";
            failDetails->push_back(d);
        }
    }

    // Service batch stop (transaction protected)
    if (!activityIds.empty()) {
        try {
            srvDAO.beginTransaction();
            int updated = srvDAO.stopOrderBatch(activityIds, stopReason, operatorId, encId, patId);
            srvDAO.commitTransaction();
            successCount += updated;
            int skipped = static_cast<int>(activityIds.size()) - updated;
            if (skipped > 0) {
                failCount += skipped;
                auto d = StopFailDetailDTO::createShared();
                d->requestId = "batch";
                d->reason = std::to_string(skipped) + " service order(s) failed to stop (may already be stopped)";
                failDetails->push_back(d);
            }
        } catch (...) {
            srvDAO.rollbackTransaction();
            failCount += static_cast<int>(activityIds.size());
            auto d = StopFailDetailDTO::createShared();
            d->requestId = "batch";
            d->reason = "Service order batch stop exception, rolled back";
            failDetails->push_back(d);
        }
    }

    // ---- Return result ----
    auto result = StopResultDTO::createShared();
    result->successCount = successCount;
    result->failCount = failCount;
    if (failDetails && failDetails->size() > 0) {
        result->failDetails = failDetails;
    }
    return result;
}
