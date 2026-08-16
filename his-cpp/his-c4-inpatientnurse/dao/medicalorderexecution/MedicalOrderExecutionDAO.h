#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/20 16:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _MEDICAL_ORDER_EXECUTION_DAO_H_
#define _MEDICAL_ORDER_EXECUTION_DAO_H_

#include "BaseDAO.h"
#include "domain/do/medicalorderexecution/ServiceRequestDO.h"
#include "domain/query/medicalorderexecution/MedicalOrderQuery.h"
#include "domain/dto/medicalorderexecution/MedicalOrderExecutionDTO.h"

struct MedicalOrderExecutionRow
{
    PtrWkfServiceRequestDO order;
    std::string orderType;
	std::string procedureId;
	std::string recordedTime;
	std::string specificationText;
	std::string dosageDisplay;
	std::string usageDisplay;
	std::string frequencyDisplay;
	std::string stopTime;
	std::string requesterName;
	std::string locationName;
	bool tcmFlag = false;
	double chineseHerbsDoseQuantity = 0;
};

using PtrMedicalOrderExecutionRow = std::shared_ptr<MedicalOrderExecutionRow>;

struct ExecutionWriteContext
{
	std::string orderType;
	std::string requestTable;
	std::string requestId;
	std::string prescriptionNo;
	std::string patientId;
	std::string encounterId;
	std::string therapyEnum;
	std::string productTable;
	std::string productId;
	std::string chargeItemDefinitionId;
	std::string locationId;
	std::string destinationLocationId;
	std::string orgId;
	std::string unitCode;
	std::string dosageInstruction;
	std::string methodCode;
	std::string frequencyCode;
	double quantity = 0;
	double unitPrice = 0;
};

/**
 * DAO for medical order execution and verification.
 * Operates on the wkf_service_request table.
 */
class MedicalOrderExecutionDAO : public BaseDAO
{
public:
    MedicalOrderExecutionDAO() = default;
    ~MedicalOrderExecutionDAO() = default;

    /**
     * Query orders pending verification (requirement 1: get verification items).
     * @param query filter conditions + pagination
     * @return list of orders awaiting verification
     */
    std::list<PtrMedicalOrderExecutionRow> queryVerificationItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Count verification items matching filter.
     * @param query filter conditions
     * @return total count
     */
    uint64_t countVerificationItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Query orders ready for execution (requirement 4: get execution items).
     * @param query filter conditions + pagination
     * @return list of orders ready for execution
     */
    std::list<PtrMedicalOrderExecutionRow> queryExecutionItems(const MedicalOrderQuery::Wrapper& query);
	std::list<PtrMedicalOrderExecutionRow> queryExecutionWorkbenchItems(
		const OrderExecutionWorkbenchQueryDTO::Wrapper& query);
	bool validExecutionAccount(const std::string& accountId,
		const std::string& encounterId);
	std::list<OrderExecutionChargeBindingDTO::Wrapper> selectExecutionBindings(
		const std::string& orderType, const std::string& requestId);
	bool loadExecutionWriteContext(const std::string& orderType,
		const std::string& requestId, const std::string& encounterId,
		const std::string& accountId, ExecutionWriteContext& context);
	bool canReserveMedication(const ExecutionWriteContext& context);
	bool canCoverExecutionAmount(const std::string& accountId, double amount);
	int insertProcedurePerformer(const std::string& id,
		const std::string& procedureId, const std::string& performerId,
		const std::string& actualTime);
	int insertExecutionDispense(const std::string& id,
		const std::string& procedureId, const ExecutionWriteContext& context,
		const std::string& planTime, const std::string& operatorId);
	int insertExecutionCharge(const std::string& id,
		const std::string& procedureId, const std::string& dispenseId,
		const ExecutionWriteContext& context, const std::string& accountId,
		const std::string& occurrenceTime, const std::string& operatorId);
	int insertExecutionBindingCharge(const std::string& id,
		const std::string& procedureId, const ExecutionWriteContext& context,
		const OrderExecutionChargeBindingDTO::Wrapper& binding,
		const std::string& accountId, const std::string& occurrenceTime,
		const std::string& operatorId);
	std::list<std::string> selectExecutionIdempotencySlots(
		const std::string& idempotencyKey);
	int insertExecutionIdempotency(const std::string& auditId,
		const std::string& idempotencyKey, const std::string& operatorId,
		const std::string& requestFingerprint, const std::string& slotMappings);

    /**
     * Count execution items matching filter.
     * @param query filter conditions
     * @return total count
     */
    uint64_t countExecutionItems(const MedicalOrderQuery::Wrapper& query);

    /**
     * Verify pass (requirement 2): update status to 'verified', record verifier and check time.
     * @param serviceRequestId order ID
     * @param verifierId nurse ID
     * @return rows affected (1=success, 0=not found/already processed)
     */
    int verifyPass(const std::string& serviceRequestId, const std::string& verifierId);

    /**
     * Return order (requirement 3): update status to 'returned', record rejection reason.
     * @param serviceRequestId order ID
     * @param returnReason explanation for rejection
     * @return rows affected (1=success, 0=not found/already processed)
     */
    int returnOrder(const std::string& serviceRequestId, const std::string& returnReason);

    /**
     * Start execution (requirement 5): update status to 'executing', record performer.
     * Status transitions from 'verified' to 'executing'.
     * @param serviceRequestId order ID
     * @param performerId nurse ID who starts execution
     * @return rows affected (1=success, 0=not found/wrong status)
     */
    int startExecution(const std::string& serviceRequestId, const std::string& performerId);

    int insertCompletedProcedure(
        const std::string& procedureId,
        const std::string& orderType,
        const std::string& requestId,
		const std::string& encounterId,
		const std::string& planTime,
		const std::string& performerId,
		const std::string& recordedTime = "");

	std::string selectActiveCompletedProcedureId(
		const std::string& orderType,
		const std::string& requestId,
		const std::string& encounterId,
		const std::string& planTime);

    int completeOrder(
        const std::string& orderType,
        const std::string& requestId,
		const std::string& encounterId,
        const std::string& performerId);

    /**
     * Complete execution (requirement 6): update status to 'completed'.
     * Status transitions from 'executing' to 'completed'.
     * @param serviceRequestId order ID
     * @param performerId nurse ID who completes execution
     * @return rows affected (1=success, 0=not found/wrong status)
     */
    int completeExecution(const std::string& serviceRequestId, const std::string& performerId);
};

#endif // !_MEDICAL_ORDER_EXECUTION_DAO_H_
