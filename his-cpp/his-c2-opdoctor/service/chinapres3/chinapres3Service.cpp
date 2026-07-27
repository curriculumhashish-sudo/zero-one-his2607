#include "stdafx.h"
#include "chinapres3Service.h"
#include "TransactionManager.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include <sstream>
#include <set>

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
			// Map database field MYSQL_SYNTHESIZE(string, updateBy, UpdateBy) assign modifier
			req->setUpdateBy(currentUser);
			// Map database field MYSQL_SYNTHESIZE(string, updateAt, UpdateAt) assign update time
			req->setUpdateAt(now);
			medReqDAO.update(*req);
		}

		// ----- 3b. INSERT Create new dispense detail record -----
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
			// Map database field MYSQL_SYNTHESIZE(string, createBy, CreateBy) assign creator
			dispDO.setCreateBy(currentUser);
			dispDO.setCreateAt(now);
			medDispDAO.insert(dispDO);

			auto presNo = req->getPrescriptionNo();
			if (!presNo.empty()) {
				processedPrescriptions.insert(presNo);
			}
		}

		// ----- 3c. Generate outpatient charge items -----
		// chinapres3ChargeItemDO assign fields: serviceTable, serviceId, patientId, encounterId,
		//   prescriptionNo, generateSourceEnum, contextEnum, orgId, createBy, createAt
		for (auto& presNo : processedPrescriptions) {
			chinapres3ChargeItemDO newCharge;
			newCharge.setId(std::to_string(snowFlake->nextId()));
			newCharge.setServiceTable("med_medication_request");
			newCharge.setServiceId(validRequests.front()->getId());
			newCharge.setPrescriptionNo(presNo);
			newCharge.setPatientId(patientId.getValue(""));
			newCharge.setEncounterId(encounterId.getValue(""));
			newCharge.setGenerateSourceEnum("TCM_ISSUE");
			newCharge.setContextEnum("OUTPATIENT");
			newCharge.setTotalPrice(0.0);
			newCharge.setQuantityValue(1.0);
			newCharge.setUnitPrice(0.0);
			newCharge.setOrgId(orgId);
			newCharge.setCreateBy(currentUser);
			newCharge.setCreateAt(now);
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