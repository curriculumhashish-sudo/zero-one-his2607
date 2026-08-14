#include "stdafx.h"
#include "MedicalOrderService.h"
#include "../../dao/medicalorder/MedicationDAO.h"
#include "../../dao/medicalorder/ServiceRequestDAO.h"
#include "TransactionManager.h"
#include <memory>

namespace {
	// 三个操作只处理请求表，不直接操作药品定义表或服务定义表。
const std::string TABLE_MEDICATION = "med_medication_request";
const std::string TABLE_SERVICE = "wkf_service_request";
	// 三个接口的合法状态流转由同一个批量执行函数统一处理。
const std::string STATUS_DRAFT = "DRAFT";
const std::string STATUS_ACTIVE = "ACTIVE";
const std::string STATUS_STOPPED = "STOPPED";
const std::string STATUS_CANCELLED = "CANCELLED";

std::string operatorId(const PayloadDTO& payload)
{
	// 优先使用认证信息中的用户ID，没有ID时再使用用户名，不能信任前端传入操作人。
    const auto id = payload.getId();
    return id.empty() ? payload.Sub() : id;
}

bool hasScope(const std::string& tenantId, const std::string& encounterId, const std::string& patientId)
{
	// 租户、就诊和患者必须同时存在，才能组成完整的医嘱归属范围。
    return !tenantId.empty() && !encounterId.empty() && !patientId.empty();
}

bool hasExpectedStatus(const std::string& table, const std::string& id,
    const std::string& tenantId, const std::string& encounterId, const std::string& patientId,
    const std::string& expectedStatus, MedicationDAO& medicationDao, ServiceRequestDAO& serviceDao)
{
	// 先查询并校验原状态，未知表名也会在这里直接失败。
    if (table == TABLE_MEDICATION) {
        auto record = medicationDao.selectByScope(id, tenantId, encounterId, patientId);
        return record && record->getStatusEnum() == expectedStatus;
    }
    if (table == TABLE_SERVICE) {
        auto record = serviceDao.selectByScope(id, tenantId, encounterId, patientId);
        return record && record->getStatusEnum() == expectedStatus;
    }
    return false;
}

int updateStatus(const std::string& table, const std::string& id,
    const std::string& tenantId, const std::string& encounterId, const std::string& patientId,
    const std::string& expectedStatus, const std::string& targetStatus, const std::string& operatorIdValue,
    MedicationDAO& medicationDao, ServiceRequestDAO& serviceDao)
{
	// 只把已知的两张请求表交给对应DAO，其他表名不允许更新。
    if (table == TABLE_MEDICATION) {
        return medicationDao.updateStatus(id, tenantId, encounterId, patientId,
            expectedStatus, targetStatus, operatorIdValue);
    }
    if (table == TABLE_SERVICE) {
        return serviceDao.updateStatus(id, tenantId, encounterId, patientId,
            expectedStatus, targetStatus, operatorIdValue);
    }
    return 0;
}

template<typename Items>
MedicalOrderOperationResultDTO::Wrapper executeStatusOperation(const Items& items,
    const std::string& tenantId, const std::string& encounterId, const std::string& patientId,
    const std::string& expectedStatus, const std::string& targetStatus, const std::string& operatorIdValue)
{
	// 先把整批目标全部校验完，再开始事务，避免批量请求只成功一部分。
	if (items.empty()) {
        return nullptr;
    }

    MedicationDAO medicationDao;
    ServiceRequestDAO serviceDao;
    for (const auto& item : items) {
        if (!item || item->id.getValue("").empty()) {
            return nullptr;
        }

        const auto table = item->productTableName.getValue("");
        if (!hasExpectedStatus(table, item->id.getValue(""), tenantId, encounterId,
            patientId, expectedStatus, medicationDao, serviceDao)) {
            return nullptr;
        }
    }

    auto result = MedicalOrderOperationResultDTO::createShared();
    std::unique_ptr<TransactionManager> transaction;
    try {
		// 药品医嘱和服务医嘱共用一个事务，混合批量也必须整体成功或整体回滚。
        transaction = std::make_unique<TransactionManager>(&medicationDao, &serviceDao);
        for (const auto& item : items) {
            const auto table = item->productTableName.getValue("");
            if (updateStatus(table, item->id.getValue(""), tenantId, encounterId,
                patientId, expectedStatus, targetStatus, operatorIdValue,
                medicationDao, serviceDao) != 1) {
                transaction->rollback();
                return nullptr;
            }
            result->updatedCount = result->updatedCount.getValue(0) + 1;
        }
        transaction->commit();
        return result;
    }
    catch (...) {
        if (transaction) {
            transaction->rollback();
        }
    }
    return nullptr;
}
}

MedicalOrderOperationResultDTO::Wrapper MedicalOrderService::sign(const SignAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 只允许把待签发医嘱从DRAFT状态改为ACTIVE状态。
    if (!dto || !dto->signTargets
        || !hasScope(dto->tenant_id.getValue(""), dto->encounter_id.getValue(""), dto->patient_id.getValue(""))
        || operatorId(payload).empty()) {
        return nullptr;
    }
    return executeStatusOperation(*dto->signTargets, dto->tenant_id.getValue(""),
        dto->encounter_id.getValue(""), dto->patient_id.getValue(""),
        STATUS_DRAFT, STATUS_ACTIVE, operatorId(payload));
}

MedicalOrderOperationResultDTO::Wrapper MedicalOrderService::withdraw(const WithdrawAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 只允许把生效中的医嘱从ACTIVE状态改为CANCELLED状态。
    if (!dto || !dto->targets
        || !hasScope(dto->tenant_id.getValue(""), dto->encounter_id.getValue(""), dto->patient_id.getValue(""))
        || operatorId(payload).empty()) {
        return nullptr;
    }
    return executeStatusOperation(*dto->targets, dto->tenant_id.getValue(""),
        dto->encounter_id.getValue(""), dto->patient_id.getValue(""),
        STATUS_ACTIVE, STATUS_CANCELLED, operatorId(payload));
}

MedicalOrderOperationResultDTO::Wrapper MedicalOrderService::cancelStop(const CancelStopAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 只允许把已停嘱医嘱从STOPPED状态恢复为ACTIVE状态。
    if (!dto || !dto->lines
        || !hasScope(dto->tenant_id.getValue(""), dto->encounter_id.getValue(""), dto->patient_id.getValue(""))
        || operatorId(payload).empty()) {
        return nullptr;
    }
    return executeStatusOperation(*dto->lines, dto->tenant_id.getValue(""),
        dto->encounter_id.getValue(""), dto->patient_id.getValue(""),
        STATUS_STOPPED, STATUS_ACTIVE, operatorId(payload));
}
