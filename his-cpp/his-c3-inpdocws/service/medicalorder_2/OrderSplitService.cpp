#include "stdafx.h"
#pragma execution_character_set("utf-8")
#include "OrderSplitService.h"
#include "dao/medicalorder_2/MedOrderDAO.h"
#include "dao/medicalorder_2/AdmEncounterDAO.h"


SplitResultDTO::Wrapper OrderSplitService::splitOrder(
    const OrderSplitDTO::Wrapper& dto)
{
    // Gate 1: Validate encounter
    std::string encId = dto->encounterId.getValue("");
    std::string patId = dto->patientId.getValue("");
    if (encId.empty() || patId.empty()) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        result->errorMessage = "Missing encounter ID or patient ID";
        return result;
    }

    AdmEncounterDAO encDAO;
    auto enc = encDAO.selectById(encId);
    if (!enc || enc->getPatientId() != patId) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        result->errorMessage = "Encounter not found or patient mismatch";
        return result;
    }

    // Gate 2: Validate operator
    std::string operatorId = dto->operatorId.getValue("");
    if (operatorId.empty()) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        result->errorMessage = "Missing operator ID";
        return result;
    }

    // Gate 3: Parameter validation
    if (!dto->orderIds || dto->orderIds->size() == 0) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        result->errorMessage = "Missing order ID list for split";
        return result;
    }

    // Collect valid ID list
    std::list<std::string> splitIds;
    for (auto& id : *dto->orderIds) {
        std::string idStr = id.getValue("");
        if (!idStr.empty()) {
            splitIds.push_back(idStr);
        }
    }

    if (splitIds.empty()) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        result->errorMessage = "Order ID list is empty";
        return result;
    }

    // Pre-check: each order exists, not STOPPED, belongs to current encounter and patient
    MedOrderDAO dao;
    for (auto& id : splitIds) {
        auto order = dao.selectById(id);
        if (!order) {
            auto result = SplitResultDTO::createShared();
            result->splitCount = 0;
            result->errorMessage = "Order " + id + " not found";
            return result;
        }
        if (order->getStatusEnum() == "STOPPED") {
            auto result = SplitResultDTO::createShared();
            result->splitCount = 0;
            result->errorMessage = "Order " + id + " already stopped, cannot split";
            return result;
        }
        if (order->getEncounterId() != encId || order->getPatientId() != patId) {
            auto result = SplitResultDTO::createShared();
            result->splitCount = 0;
            result->errorMessage = "Order " + id + " does not belong to current encounter or patient";
            return result;
        }
    }

    // Execute database update (with transaction protection)
    try {
        dao.beginTransaction();
        int affected = dao.clearGroupIdBatch(splitIds, encId, patId);
        dao.commitTransaction();

        auto result = SplitResultDTO::createShared();
        result->splitCount = affected;
        return result;
    } catch (...) {
        dao.rollbackTransaction();
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        result->errorMessage = "Split operation exception";
        return result;
    }
}
