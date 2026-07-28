#include "stdafx.h"
#include "chinapres3Service.h"
#include "TransactionManager.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include <sstream>
#include <set>
#include <map>

// ===================================================================
//                         Revoke Implementation
// ===================================================================

chinapres3RevokeResultVO::Wrapper chinapres3Service::revoke(
	const chinapres3RevokeDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto resultVo = chinapres3RevokeResultVO::createShared();

	// ---- 1. Parameter Verification ----
	auto prescriptionNos = dto->prescriptionNos;
	if (!prescriptionNos || prescriptionNos->size() == 0) {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass prescription numbers";
		return resultVo;
	}

	auto now = SimpleDateTimeFormat::format();
	auto currentUser = payload.getUsername();

	v_int32 revokedPresCount = 0;
	v_int32 revokedMedCount = 0;
	auto failureList = oatpp::Vector<oatpp::Object<chinapres3RevokeFailureDTO>>::createShared();

	// ---- 2. Process each prescription independently ----
	for (auto& presNo : *prescriptionNos) {
		auto presNoStr = std::string(presNo.getValue(""));
		if (presNoStr.empty()) continue;

		try {
			// Each prescription gets its own transaction
			TransactionManager tm(&medReqDAO, &medDispDAO, &chargeDAO);

			// ---- 2a. Query ACTIVE medication requests for this prescription ----
			auto reqQuery = chinapres3MedicationRequestQuery::createShared();
			reqQuery->prescriptionNo = presNoStr;
			reqQuery->statusEnum = "ACTIVE";
			reqQuery->pageIndex = 1;
			reqQuery->pageSize = 1000;
			auto requests = medReqDAO.selectWithPage(reqQuery);

			if (requests.empty()) {
				tm.rollback();
				auto failure = chinapres3RevokeFailureDTO::createShared();
				failure->prescriptionNo = presNoStr;
				failure->reason = "Prescription not found or not in issued status";
				failureList->push_back(failure);
				continue;
			}

			// ---- 2b. Verify dispense records: must be PREPARATION and not deleted ----
			std::vector<Ptrchinapres3MedicationDispenseDO> dispensesToUpdate;
			bool dispenseFailed = false;

			for (auto& req : requests) {
				auto dispQuery = chinapres3MedicationDispenseQuery::createShared();
				dispQuery->medReqId = req->getId();
				dispQuery->statusEnum = "PREPARATION";
				dispQuery->isDeleted = "0";
				dispQuery->pageIndex = 1;
				dispQuery->pageSize = 1000;
				auto dispList = medDispDAO.selectWithPage(dispQuery);

				if (dispList.empty()) {
					dispenseFailed = true;
					break;
				}

				for (auto& d : dispList) {
					dispensesToUpdate.push_back(d);
				}
			}

			if (dispenseFailed) {
				tm.rollback();
				auto failure = chinapres3RevokeFailureDTO::createShared();
				failure->prescriptionNo = presNoStr;
				failure->reason = "Some medication has been dispensed or already revoked, cannot revoke";
				failureList->push_back(failure);
				continue;
			}

			// ---- 2c. UPDATE medication_request: status_enum �� DRAFT ----
			for (auto& req : requests) {
				req->setStatusEnum("DRAFT");
				// rx_type_code stays "TCM" unchanged
				req->setUpdatedBy(currentUser);
				req->setUpdatedAt(now);
				medReqDAO.update(*req);
			}

			// ---- 2d. UPDATE medication_dispense: is_deleted = 1 ----
			for (auto& d : dispensesToUpdate) {
				d->setIsDeleted("1");
				d->setUpdatedBy(currentUser);
				d->setUpdatedAt(now);
				medDispDAO.update(*d);
			}

			// ---- 2e. UPDATE adm_charge_item: is_deleted = 1 ----
			auto chargeQuery = chinapres3ChargeItemQuery::createShared();
			chargeQuery->prescriptionNo = presNoStr;
			chargeQuery->isDeleted = "0";
			chargeQuery->pageIndex = 1;
			chargeQuery->pageSize = 1000;
			auto charges = chargeDAO.selectWithPage(chargeQuery);

			for (auto& c : charges) {
				c->setIsDeleted("1");
				c->setUpdatedBy(currentUser);
				c->setUpdatedAt(now);
				chargeDAO.update(*c);
			}

			// ---- Commit this prescription's transaction ----
			tm.commit();

			revokedPresCount++;
			revokedMedCount += static_cast<v_int32>(requests.size());

		}
		catch (const std::exception& e) {
			auto failure = chinapres3RevokeFailureDTO::createShared();
			failure->prescriptionNo = presNoStr;
			std::stringstream errMsg;
			errMsg << "Transaction failed: " << e.what();
			failure->reason = errMsg.str();
			failureList->push_back(failure);
		}
	}

	// ---- 3. Assemble result ----
	auto resultData = chinapres3RevokeResultDTO::createShared();
	resultData->revokedPrescriptionCount = revokedPresCount;
	resultData->revokedMedicationCount = revokedMedCount;
	resultData->failures = failureList;

	resultVo->success(resultData);
	resultVo->message = "Revoke completed";
	return resultVo;
}

// ===================================================================
//                         Issue Implementation
// ===================================================================

chinapres3IssueResultVO::Wrapper chinapres3Service::issue(
	const chinapres3IssueDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto resultVo = chinapres3IssueResultVO::createShared();

	// ==================== 1. Parameter Verification ====================
	auto encounterId = dto->encounterId;
	if (!encounterId || encounterId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass encounter ID";
		return resultVo;
	}
	auto patientId = dto->patientId;
	if (!patientId || patientId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass patient ID";
		return resultVo;
	}
	auto signTargets = dto->signTargets;
	if (!signTargets || signTargets->size() == 0) {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass prescription list to sign";
		return resultVo;
	}

	// ==================== 2. Loop to verify prescription list to dispense ====================
	std::vector<Ptrchinapres3MedicationRequestDO> validRequests;
	std::stringstream errStream;

	for (auto& target : *signTargets) {
		auto medReqId = target->medicationRequestId;
		if (!medReqId || medReqId.getValue("") == "") {
			resultVo->code = 400;
			resultVo->message = "Parameter cannot be empty, please pass prescription ID";
			return resultVo;
		}
		auto req = medReqDAO.selectById(medReqId.getValue(""));
		if (!req) {
			errStream << "Prescription record not found, prescription ID=" << medReqId.getValue("");
			resultVo->code = 404;
			resultVo->message = errStream.str();
			return resultVo;
		}
		auto status = req->getStatusEnum();
		if (status != "DRAFT") {
			errStream << "Prescription status disallows dispensing, current status=" << status
				<< ", prescription ID=" << medReqId.getValue("");
			resultVo->code = 400;
			resultVo->message = errStream.str();
			return resultVo;
		}
		auto reqEncounterId = req->getEncounterId();
		if (!reqEncounterId.empty() && reqEncounterId != encounterId.getValue("")) {
			resultVo->code = 400;
			resultVo->message = "Encounter ID mismatches prescription, prescription ID="
				+ medReqId.getValue("");
			return resultVo;
		}
		auto reqPatientId = req->getPatientId();
		if (!reqPatientId.empty() && reqPatientId != patientId.getValue("")) {
			resultVo->code = 400;
			resultVo->message = "Patient ID mismatches prescription, prescription ID="
				+ medReqId.getValue("");
			return resultVo;
		}
		validRequests.push_back(req);
	}

	// ==================== 3. Update prescription, generate dispense & charge records within transaction ====================
	try
	{
		TransactionManager tm(&medReqDAO, &medDispDAO, &chargeDAO);
		auto now = SimpleDateTimeFormat::format();
		auto snowFlake = SnowFlake::getInstance();
		auto currentUser = payload.getUsername();
		auto orgId = payload.getId();

		std::vector<std::string> busNos;
		std::set<std::string> processedPrescriptions;

		// ----- 3a. UPDATE Set prescription status to ACTIVE -----
		for (auto& req : validRequests) {
			req->setStatusEnum("ACTIVE");
			req->setRxTypeCode("TCM");
			req->setUpdatedBy(currentUser);
			req->setUpdatedAt(now);
			medReqDAO.update(*req);
		}

		// ----- 3b. INSERT Create new dispense detail record -----
		// prescriptionNo �� first request ID mapping for charge generation
		std::map<std::string, std::string> presNoToReqId;
		for (auto& req : validRequests) {
			chinapres3MedicationDispenseDO dispDO;
			dispDO.setId(std::to_string(snowFlake->nextId()));
			auto busNo = "BUS" + std::to_string(snowFlake->nextId());
			dispDO.setBusNo(busNo);
			busNos.push_back(busNo);
			dispDO.setStatusEnum("PREPARATION");
			dispDO.setDispenseEnum("OUTPATIENT");
			dispDO.setMedReqId(req->getId());
			dispDO.setPatientId(patientId.getValue(""));
			dispDO.setEncounterId(encounterId.getValue(""));
			// Copy medication detail from request for NOT NULL fields
			dispDO.setMedicationId(req->getMedicationId());
			dispDO.setQuantity(req->getQuantity());
			dispDO.setUnitCode(req->getUnitCode());
			dispDO.setLocationId(req->getLocationId());
			dispDO.setCreatedBy(currentUser);
			dispDO.setCreatedAt(now);
			medDispDAO.insert(dispDO);

			auto presNo = req->getPrescriptionNo();
			if (!presNo.empty()) {
				processedPrescriptions.insert(presNo);
				// Record first request for each prescription for charge generation
				if (presNoToReqId.find(presNo) == presNoToReqId.end()) {
					presNoToReqId[presNo] = req->getId();
				}
			}
		}

		// ----- 3c. Generate outpatient charge items -----
		for (auto& presNo : processedPrescriptions) {
			chinapres3ChargeItemDO newCharge;
			newCharge.setId(std::to_string(snowFlake->nextId()));
			newCharge.setServiceTable("med_medication_request");
			newCharge.setServiceId(presNoToReqId[presNo]);
			newCharge.setPrescriptionNo(presNo);
			newCharge.setPatientId(patientId.getValue(""));
			newCharge.setEncounterId(encounterId.getValue(""));
			newCharge.setGenerateSourceEnum("TCM_ISSUE");
			newCharge.setContextEnum("OUTPATIENT");
			newCharge.setTotalPrice(0.0);
			newCharge.setQuantityValue(1.0);
			newCharge.setUnitPrice(0.0);
			newCharge.setOrgId(orgId);
			newCharge.setCreatedBy(currentUser);
			newCharge.setCreatedAt(now);
			chargeDAO.insert(newCharge);
		}

		// ----- 3d. Commit database transaction -----
		tm.commit();

		// ----- 4. Assemble response result data -----
		auto resultData = chinapres3IssueResultDTO::createShared();
		resultData->processedCount = static_cast<oatpp::Int32>(validRequests.size());
		resultData->prescriptionCount = static_cast<oatpp::Int32>(processedPrescriptions.size());
		auto busNoList = oatpp::Vector<oatpp::String>::createShared();
		for (auto& bno : busNos) {
			busNoList->push_back(oatpp::String(bno.c_str()));
		}
		resultData->busNos = busNoList;

		resultVo->success(resultData);
		resultVo->message = "Prescription issued successfully";
	}
	catch (const std::exception& e)
	{
		resultVo->code = 500;
		std::stringstream errMsg;
		errMsg << "Exception occurred during prescription issue transaction: " << e.what();
		resultVo->message = errMsg.str();
	}

	return resultVo;
}