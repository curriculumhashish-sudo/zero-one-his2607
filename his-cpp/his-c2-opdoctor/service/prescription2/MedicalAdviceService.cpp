#include "stdafx.h"
#include "MedicalAdviceService.h"
#include "dao/prescription2/MedicationRequestDAO.h"
#include "dao/prescription2/ServiceRequestDAO.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include <set>

namespace
{
// 医嘱签发流程允许的状态。
const std::string MEDICAL_ADVICE_DRAFT_STATUS = "DRAFT";
const std::string MEDICAL_ADVICE_COMPLETED_STATUS = "COMPLETED";
}

std::list<std::string> MedicalAdviceService::normalizeMedicalAdviceIds(
    const oatpp::List<oatpp::String>& medicalAdviceIds) const
{
    // 保留请求顺序，并确保每个标识只执行一次。
    std::list<std::string> ids;
    std::set<std::string> uniqueIds;
    if (!medicalAdviceIds)
    {
        return ids;
    }

    for (const auto& medicalAdviceId : *medicalAdviceIds)
    {
        const std::string id = medicalAdviceId.getValue("");
        if (!id.empty() && uniqueIds.insert(id).second)
        {
            ids.emplace_back(id);
        }
    }
    return ids;
}

std::list<std::string> MedicalAdviceService::signMedicalAdvice(
    const SignMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
    // 空批次无需执行状态流转。
    const auto ids = normalizeMedicalAdviceIds(dto->medicalAdviceIds);
    if (ids.empty())
    {
        return {};
    }

    MedicationRequestDAO dao;
    TransactionManager transaction(&dao);
    const std::string currentTime = SimpleDateTimeFormat::format();
    std::list<std::string> signedIds;
    try
    {
        for (const auto& id : ids)
        {
            // 锁定记录，避免其状态在事务期间发生变化。
            auto medicalAdvice = dao.selectByIdForUpdate(id);
            if (!medicalAdvice || medicalAdvice->getStatusEnum() != MEDICAL_ADVICE_DRAFT_STATUS)
            {
                transaction.rollback();
                return {};
            }

            // 请求传入的是 ID 列表，因此为当前记录构建一个更新对象。
            MedicationRequestDO updateData;
            updateData.setId(id);
            updateData.setStatusEnum(MEDICAL_ADVICE_COMPLETED_STATUS);
            updateData.setStatusChangedTime(currentTime);
            updateData.setPerformerCheckId(payload.getId());
            updateData.setCheckTime(currentTime);
            updateData.setUpdatedBy(payload.getUsername());
            updateData.setUpdatedAt(currentTime);
            if (dao.update(updateData) != 1)
            {
                transaction.rollback();
                return {};
            }
            signedIds.emplace_back(id);
        }
    }
    catch (...)
    {
        transaction.rollback();
        return {};
    }

    transaction.commit();
    return signedIds;
}

std::list<std::string> MedicalAdviceService::withdrawMedicalAdvice(
    const WithdrawMedicalAdviceDTO::Wrapper& dto, const PayloadDTO& payload)
{
    // 空批次无需执行状态流转。
    const auto ids = normalizeMedicalAdviceIds(dto->medicalAdviceIds);
    if (ids.empty())
    {
        return {};
    }

    MedicationRequestDAO dao;
    TransactionManager transaction(&dao);
    const std::string currentTime = SimpleDateTimeFormat::format();
    std::list<std::string> withdrawnIds;
    try
    {
        for (const auto& id : ids)
        {
            // 锁定记录，避免其状态在事务期间发生变化。
            auto medicalAdvice = dao.selectByIdForUpdate(id);
            if (!medicalAdvice || medicalAdvice->getStatusEnum() != MEDICAL_ADVICE_COMPLETED_STATUS)
            {
                transaction.rollback();
                return {};
            }

            // 请求传入的是 ID 列表，因此为当前记录构建一个更新对象。
            MedicationRequestDO updateData;
            updateData.setId(id);
            updateData.setStatusEnum(MEDICAL_ADVICE_DRAFT_STATUS);
            updateData.setStatusChangedTime(currentTime);
            updateData.setUpdatedBy(payload.getUsername());
            updateData.setUpdatedAt(currentTime);
            if (dao.update(updateData) != 1)
            {
                transaction.rollback();
                return {};
            }
            withdrawnIds.emplace_back(id);
        }
    }
    catch (...)
    {
        transaction.rollback();
        return {};
    }

    transaction.commit();
    return withdrawnIds;
}

std::string MedicalAdviceService::printApplicationForm(
    const PrintApplicationFormDTO::Wrapper& dto, const PayloadDTO& payload)
{
    // 打印操作必须指定一个服务申请单。
    const std::string serviceRequestId = dto->serviceRequestId.getValue("");
    if (serviceRequestId.empty())
    {
        return "";
    }

    ServiceRequestDAO dao;
    TransactionManager transaction(&dao);
    try
    {
        // 锁定记录，避免并发打印请求丢失次数更新。
        auto serviceRequest = dao.selectByIdForUpdate(serviceRequestId);
        if (!serviceRequest)
        {
            transaction.rollback();
            return "";
        }

        serviceRequest->setPrintCount(serviceRequest->getPrintCount() + 1);
        serviceRequest->setUpdatedBy(payload.getUsername());
        serviceRequest->setUpdatedAt(SimpleDateTimeFormat::format());
        if (dao.update(*serviceRequest) != 1)
        {
            transaction.rollback();
            return "";
        }
    }
    catch (...)
    {
        transaction.rollback();
        return "";
    }

    transaction.commit();
    return serviceRequestId;
}
