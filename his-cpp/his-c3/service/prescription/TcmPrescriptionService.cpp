/*
 Copyright (c) 2025-2066, Zero One Technology Co., Ltd.

 Licensed under the Apache License,
 * Version 2.0 (the "License");
 you may not use this file except in compliance with the License.

 * You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless
 * required by applicable law or agreed to in writing, software
 distributed under the License is
 * distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 See the License for the specific language governing permissions and
 limitations under
 * the License.
*/

#include "stdafx.h"
#include "TcmPrescriptionService.h"
#include "../../dao/prescription/TcmPrescriptionDAO.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "id/UuidFacade.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <vector>

namespace
{
// Defines the domain values shared by prescription state transitions.
const std::string COMPLETED_STATUS = "COMPLETED";
const std::string DRAFT_STATUS = "DRAFT";
const std::string STOPPED_STATUS = "STOPPED";
const std::string LONG_TERM_THERAPY = "LONG_TERM";

// Uses the authenticated user ID when available and falls back to the username.
std::string getOperatorId(const PayloadDTO &payload)
{
    const std::string operatorId = payload.getId();
    return operatorId.empty() ? payload.getUsername() : operatorId;
}

// Verifies that a locked TCM record belongs to the requested encounter.
bool matchesContext(const PtrMedMedicationRequestDO &current, const std::string &patientId,
                    const std::string &encounterId)
{
    return current && current->getPatientId() == patientId &&
           current->getEncounterId() == encounterId && current->getIsDeleted() == 0;
}
// Generates a traceable prescription number with a random suffix.
std::string generatePrescriptionNo()
{
    UuidFacade uuidFacade;
    std::string suffix = uuidFacade.genUuid();

    if (suffix.size() > 6)
    {
        suffix = suffix.substr(suffix.size() - 6);
    }

    std::transform(
        suffix.begin(),
        suffix.end(),
        suffix.begin(),
        [](unsigned char value)
        {
            return static_cast<char>(std::toupper(value));
        }
    );

    return "CP" +
        SimpleDateTimeFormat::format("%Y%m%d%H%M%S") +
        suffix;
}
} // namespace

/**
 * Signs draft prescription requests atomically by prescription group.
 */
TcmPrescriptionSignServiceResult TcmPrescriptionService::signPrescription(
    const TcmPrescriptionSignDTO::Wrapper& dto,
    const PayloadDTO& payload
)
{
    TcmPrescriptionSignServiceResult result;
    result.data = TcmPrescriptionSignResultDTO::createShared();
    result.data->success = false;
    result.data->prescriptionNoByGroupKey = {};

    if (!dto)
    {
        result.invalidRequest = true;
        result.message = "The request body is required.";
        return result;
    }

    const std::string validationError = dto->validate();
    if (!validationError.empty())
    {
        result.invalidRequest = true;
        result.message = validationError;
        return result;
    }

    const std::string patientId = dto->patientId.getValue("");
    const std::string encounterId = dto->encounterId.getValue("");

    // Sort request IDs to keep the later database lock order stable.
    std::map<std::string, std::string> groupKeyByRequestId;

    for (const auto& target : *dto->signTargets)
    {
        const std::string requestId =
            target->medicationRequestId.getValue("");
        const std::string groupKey =
            target->prescriptionGroupKey.getValue("");

        const auto inserted =
            groupKeyByRequestId.emplace(requestId, groupKey);

        if (!inserted.second)
        {
            result.invalidRequest = true;
            result.message =
                "A medication request cannot be signed more than once.";
            return result;
        }
    }

    // Collect locked rows by group before performing any update.
    std::map<
        std::string,
        std::vector<PtrMedMedicationRequestDO>
    > requestsByGroup;

    try
    {
        TcmPrescriptionDAO dao;
        TransactionManager transaction(&dao);

        try
        {
            for (const auto& targetEntry : groupKeyByRequestId)
            {
                const std::string& requestId = targetEntry.first;
                const std::string& groupKey = targetEntry.second;

                auto current = dao.selectByIdForUpdate(requestId);

                if (!matchesContext(current, patientId, encounterId))
                {
                    transaction.rollback();
                    result.message =
                        "A medication request was not found in this encounter.";
                    return result;
                }

                if (current->getStatusEnum() != DRAFT_STATUS ||
                    current->getPrescriptionGroupKey() != groupKey)
                {
                    transaction.rollback();
                    result.message =
                        "A medication request is not eligible for signing.";
                    return result;
                }

                requestsByGroup[groupKey].push_back(current);
            }

            const std::string changedTime =
                SimpleDateTimeFormat::format();
            const std::string operatorId =
                getOperatorId(payload);

            auto generatedPrescriptionNos =
                oatpp::Fields<oatpp::String>::createShared();
            // Reuse the number of a withdrawn draft, or generate one for a new draft.
            for (const auto& groupEntry : requestsByGroup)
            {
                const std::string& groupKey = groupEntry.first;
                const auto& groupRequests = groupEntry.second;
                std::string prescriptionNo;

                for (const auto& current : groupRequests)
                {
                    if (current->getPrescriptionNo().empty())
                        continue;

                    if (prescriptionNo.empty())
                    {
                        prescriptionNo = current->getPrescriptionNo();
                    }
                    else if (prescriptionNo != current->getPrescriptionNo())
                    {
                        transaction.rollback();
                        result.message =
                            "The draft prescription group has inconsistent numbers.";
                        return result;
                    }
                }

                if (prescriptionNo.empty())
                    prescriptionNo = generatePrescriptionNo();

                for (const auto& current : groupRequests)
                {
                    const int affectedRows = dao.updateForSign(
                        *current,
                        prescriptionNo,
                        COMPLETED_STATUS,
                        changedTime,
                        operatorId,
                        DRAFT_STATUS
                    );

                    if (affectedRows != 1)
                    {
                        transaction.rollback();
                        result.message =
                            "The prescription changed concurrently. "
                            "Please retry.";
                        return result;
                    }
                }

                generatedPrescriptionNos[
                    groupKey.c_str()
                ] = prescriptionNo.c_str();
            }

            // Report success only after all groups have been committed.
            transaction.commit();
            result.data->prescriptionNoByGroupKey =
                generatedPrescriptionNos;
            result.success = true;
            result.data->success = true;
            result.message =
                "The prescription was signed successfully.";
            return result;
        }
        catch (...)
        {
            transaction.rollback();
            throw;
        }
    }
    catch (...)
    {
        result.message = "The database operation failed.";
        return result;
    }
}
/**
 * Withdraws a completed prescription after validating all locked request records.
 */
TcmPrescriptionWithdrawServiceResult TcmPrescriptionService::withdrawPrescription(
    const TcmPrescriptionWithdrawDTO::Wrapper &dto, const PayloadDTO &payload)
{
    TcmPrescriptionWithdrawServiceResult result;
    result.data = TcmPrescriptionWithdrawResultDTO::createShared();
    result.data->success = false;

    if (!dto || !dto->medicationRequestIds || dto->medicationRequestIds->empty())
    {
        result.invalidRequest = true;
        result.message = "The request body and medication request IDs are required.";
        return result;
    }
    const std::string patientId = dto->patientId.getValue("");
    const std::string encounterId = dto->encounterId.getValue("");
    const std::string action = dto->action.getValue("");
    const std::string scope = dto->scope.getValue("");
    const std::string prescriptionNo = dto->prescriptionNo.getValue("");
    const std::string prescriptionGroupKey = dto->prescriptionGroupKey.getValue("");

    if (patientId.empty() || encounterId.empty() || prescriptionNo.empty() ||
        prescriptionGroupKey.empty() || action != "WITHDRAW" || scope != "PRESCRIPTION")
    {
        result.invalidRequest = true;
        result.message = "The withdrawal context is incomplete or invalid.";
        return result;
    }

    std::set<std::string> uniqueIds;
    std::vector<PtrMedMedicationRequestDO> requests;

    try
    {
        TcmPrescriptionDAO dao;
        TransactionManager transaction(&dao);

        try
        {
            // Lock and validate the complete batch before changing any record.
            for (const auto &wrappedId : *dto->medicationRequestIds)
            {
                const std::string requestId = wrappedId.getValue("");

                if (requestId.empty() || !uniqueIds.insert(requestId).second)
                {
                    transaction.rollback();
                    result.invalidRequest = true;
                    result.message = "Medication request IDs must be non-empty and unique.";
                    return result;
                }
                auto current = dao.selectByIdForUpdate(requestId);

                if (!matchesContext(current, patientId, encounterId))
                {
                    transaction.rollback();
                    result.message = "A medication request was not found in this encounter.";
                    return result;
                }
                if (current->getStatusEnum() != COMPLETED_STATUS ||
                    current->getPrescriptionNo() != prescriptionNo ||
                    current->getPrescriptionGroupKey() != prescriptionGroupKey)
                {
                    transaction.rollback();
                    result.message = "The prescription is not in a withdrawable state.";
                    return result;
                }
                if (dao.countWithdrawalBlockingRecords(requestId, prescriptionNo,
                                                       current->getTenantId()) != 0)
                {
                    transaction.rollback();
                    result.message =
                        "The prescription has dispensing, charging, or execution records.";
                    return result;
                }

                requests.push_back(current);
            }
            const std::string changedTime = SimpleDateTimeFormat::format();
            const std::string operatorId = getOperatorId(payload);

            // Update every record with optimistic status and version guards.
            for (const auto &current : requests)
            {
                if (dao.updateForWithdraw(*current, DRAFT_STATUS, changedTime, operatorId,
                                          COMPLETED_STATUS) != 1)
                {
                    transaction.rollback();
                    result.message = "The prescription changed concurrently. Please retry.";
                    return result;
                }
            }

            // Report success only after the complete batch has been committed.
            transaction.commit();
            result.success = true;
            result.data->success = true;
            result.message = "The prescription was withdrawn successfully.";
            return result;
        }
        catch (...)
        {
            transaction.rollback();
            throw;
        }
    }
    catch (...)
    {
        result.message = "The database operation failed.";
        return result;
    }
}

/**
 * Stops completed long-term prescriptions in one atomic transaction.
 */
TcmPrescriptionStopServiceResult TcmPrescriptionService::stopPrescription(
    const TcmPrescriptionStopDTO::Wrapper &dto, const PayloadDTO &payload)
{
    TcmPrescriptionStopServiceResult result;
    result.data = TcmPrescriptionStopResultDTO::createShared();
    result.data->success = false;

    if (!dto || !dto->prescriptionNos || dto->prescriptionNos->empty())
    {
        result.invalidRequest = true;
        result.message = "The request body and prescription numbers are required.";
        result.data->message = result.message.c_str();
        return result;
    }
    const std::string patientId = dto->patientId.getValue("");
    const std::string encounterId = dto->encounterId.getValue("");

    if (patientId.empty() || encounterId.empty())
    {
        result.invalidRequest = true;
        result.message = "The patient and encounter identifiers are required.";
        result.data->message = result.message.c_str();
        return result;
    }
    std::set<std::string> uniquePrescriptionNos;
    std::vector<PtrMedMedicationRequestDO> requests;

    try
    {
        TcmPrescriptionDAO dao;
        TransactionManager transaction(&dao);

        try
        {
            // Lock and validate every detail row for each requested prescription number.
            for (const auto &wrappedNo : *dto->prescriptionNos)
            {
                const std::string prescriptionNo = wrappedNo.getValue("");

                if (prescriptionNo.empty() ||
                    !uniquePrescriptionNos.insert(prescriptionNo).second)
                {
                    transaction.rollback();
                    result.invalidRequest = true;
                    result.message = "Prescription numbers must be non-empty and unique.";
                    result.data->message = result.message.c_str();
                    return result;
                }
                const auto rows =
                    dao.selectByPrescriptionNoForUpdate(prescriptionNo, patientId, encounterId);

                if (rows.empty())
                {
                    transaction.rollback();
                    result.message = "A prescription was not found.";
                    result.data->message = result.message.c_str();
                    return result;
                }
                for (const auto &current : rows)
                {
                    if (!matchesContext(current, patientId, encounterId) ||
                        current->getStatusEnum() != COMPLETED_STATUS ||
                        current->getTherapyEnum() != LONG_TERM_THERAPY)
                    {
                        transaction.rollback();
                        result.message =
                            "Only completed long-term prescriptions can be stopped.";
                        result.data->message = result.message.c_str();
                        return result;
                    }

                    requests.push_back(current);
                }
            }
            const std::string changedTime = SimpleDateTimeFormat::format();
            const std::string requestedStopTime = dto->stopTime.getValue("");
            const std::string stopTime =
                requestedStopTime.empty() ? changedTime : requestedStopTime;
            const std::string operatorId = getOperatorId(payload);

            // Apply the stop transition with optimistic status and version guards.
            for (const auto &current : requests)
            {
                if (dao.updateForStop(*current, STOPPED_STATUS, stopTime, changedTime, operatorId,
                                      COMPLETED_STATUS) != 1)
                {
                    transaction.rollback();
                    result.message = "The prescription changed concurrently. Please retry.";
                    result.data->message = result.message.c_str();
                    return result;
                }
            }

            // Report success only after the complete batch has been committed.
            transaction.commit();
            result.success = true;
            result.data->success = true;
            result.message = "The prescription was stopped successfully.";
            result.data->message = result.message.c_str();
            return result;
        }
        catch (...)
        {
            transaction.rollback();
            throw;
        }
    }
    catch (...)
    {
        result.message = "The database operation failed.";
        result.data->message = result.message.c_str();
        return result;
    }
}

/**
 * Cancels previous stops and restores eligible prescriptions atomically.
 */
TcmPrescriptionCancelStopServiceResult TcmPrescriptionService::cancelStopPrescription(
    const TcmPrescriptionCancelStopDTO::Wrapper &dto, const PayloadDTO &payload)
{
    TcmPrescriptionCancelStopServiceResult result;
    result.data = TcmPrescriptionCancelStopResultDTO::createShared();
    result.data->success = false;

    if (!dto || !dto->prescriptionNos || dto->prescriptionNos->empty())
    {
        result.invalidRequest = true;
        result.message = "The request body and prescription numbers are required.";
        result.data->message = result.message.c_str();
        return result;
    }
    const std::string patientId = dto->patientId.getValue("");
    const std::string encounterId = dto->encounterId.getValue("");

    if (patientId.empty() || encounterId.empty())
    {
        result.invalidRequest = true;
        result.message = "The patient and encounter identifiers are required.";
        result.data->message = result.message.c_str();
        return result;
    }

    std::set<std::string> uniquePrescriptionNos;
    std::vector<PtrMedMedicationRequestDO> requests;

    try
    {
        TcmPrescriptionDAO dao;
        TransactionManager transaction(&dao);

        try
        {
            // Lock and validate every detail row for each requested prescription number.
            for (const auto &wrappedNo : *dto->prescriptionNos)
            {
                const std::string prescriptionNo = wrappedNo.getValue("");

                if (prescriptionNo.empty() ||
                    !uniquePrescriptionNos.insert(prescriptionNo).second)
                {
                    transaction.rollback();
                    result.invalidRequest = true;
                    result.message = "Prescription numbers must be non-empty and unique.";
                    result.data->message = result.message.c_str();
                    return result;
                }
                const auto rows =
                    dao.selectByPrescriptionNoForUpdate(prescriptionNo, patientId, encounterId);

                if (rows.empty())
                {
                    transaction.rollback();
                    result.message = "A prescription was not found.";
                    result.data->message = result.message.c_str();
                    return result;
                }
                for (const auto &current : rows)
                {
                    if (!matchesContext(current, patientId, encounterId) ||
                        current->getStatusEnum() != STOPPED_STATUS ||
                        current->getTherapyEnum() != LONG_TERM_THERAPY)
                    {
                        transaction.rollback();
                        result.message =
                            "Only stopped long-term prescriptions can have their stop canceled.";
                        result.data->message = result.message.c_str();
                        return result;
                    }

                    requests.push_back(current);
                }
            }
            const std::string changedTime = SimpleDateTimeFormat::format();
            const std::string operatorId = getOperatorId(payload);

            // Restore every record with optimistic status and version guards.
            for (const auto &current : requests)
            {
                if (dao.updateForCancelStop(*current, COMPLETED_STATUS, changedTime, operatorId,
                                            STOPPED_STATUS) != 1)
                {
                    transaction.rollback();
                    result.message = "The prescription changed concurrently. Please retry.";
                    result.data->message = result.message.c_str();
                    return result;
                }
            }

            // Report success only after the complete batch has been committed.
            transaction.commit();
            result.success = true;
            result.data->success = true;
            result.message = "The prescription stop was canceled successfully.";
            result.data->message = result.message.c_str();
            return result;
        }
        catch (...)
        {
            transaction.rollback();
            throw;
        }
    }
    catch (...)
    {
        result.message = "The database operation failed.";
        result.data->message = result.message.c_str();
        return result;
    }
}
