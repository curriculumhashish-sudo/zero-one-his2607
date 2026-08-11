#include "stdafx.h"
#include "OrderSplitService.h"
#include "dao/medicalorder/MedOrderDAO.h"
#include "dao/medicalorder/AdmEncounterDAO.h"


SplitResultDTO::Wrapper OrderSplitService::splitOrder(
    const OrderSplitDTO::Wrapper& dto)
{
    //Gate 1: 就诊门控
    std::string encId = dto->encounterId.getValue("");
    std::string patId = dto->patientId.getValue("");
    if (encId.empty() || patId.empty()) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        return result;
    }

    AdmEncounterDAO encDAO;
    auto enc = encDAO.selectById(encId);
    if (!enc || enc->getPatientId() != patId) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        return result;
    }

    //Gate 2: 操作人
    std::string operatorId = dto->operatorId.getValue("");
    if (operatorId.empty()) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        return result;
    }

    //Gate 3: 参数校验
    if (!dto->orderIds || dto->orderIds->size() == 0) {
        auto result = SplitResultDTO::createShared();
        result->splitCount = 0;
        return result;
    }

    //收集ID列表
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
        return result;
    }

    //前置校验：每条order存在且非STOPPED
    MedOrderDAO dao;
    for (auto& id : splitIds) {
        auto order = dao.selectById(id);
        if (!order) {
            auto result = SplitResultDTO::createShared();
            result->splitCount = 0;
            return result;
        }
        if (order->getStatusEnum() == "STOPPED") {
            auto result = SplitResultDTO::createShared();
            result->splitCount = 0;
            return result;
        }
    }

    // 执行拆组
    int affected = dao.clearGroupIdBatch(splitIds);

    auto result = SplitResultDTO::createShared();
    result->splitCount = affected;
    return result;
}