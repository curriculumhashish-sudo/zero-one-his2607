#include "stdafx.h"
#include "chinapres3Service.h"
#include "TransactionManager.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include <sstream>
#include <set>
#include <map>

// ===================================================================
//                         处方撤回实现 Revoke
// ===================================================================

/**
 * @brief 处方撤回接口业务逻辑
 * @param dto 处方撤回入参DTO，包含待撤回处方编号列表
 * @param payload JWT身份载体，携带当前登录用户、租户等信息
 * @return chinapres3RevokeResultVO::Wrapper 撤回结果VO，包含成功计数、失败清单
 * 业务规则：
 * 1. 多条处方独立事务，单张处方失败不影响其他处方处理
 * 2. 仅允许撤回【已签发(ACTIVE)】且【未发药(PREPARATION)】的处方
 * 3. 执行逻辑：更新用药申请状态 → 逻辑删除发药记录 → 逻辑删除收费明细
 */
chinapres3RevokeResultVO::Wrapper chinapres3Service::revoke(
	const chinapres3RevokeDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto resultVo = chinapres3RevokeResultVO::createShared();

	// ---- 1. 参数校验 ----
	auto prescriptionNos = dto->prescriptionNos;
	if (!prescriptionNos || prescriptionNos->size() == 0) {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass prescription numbers";
		return resultVo;
	}

	auto now = SimpleDateTimeFormat::format();
	auto currentUser = payload.getUsername();

	v_int32 revokedPresCount = 0;         // 成功撤回处方数量
	v_int32 revokedMedCount = 0;          // 成功撤回药品条目数量
	auto failureList = oatpp::Vector<oatpp::Object<chinapres3RevokeFailureDTO>>::createShared();

	// ---- 2. 循环逐条处理处方（每张处方独立事务） ----
	for (auto& presNo : *prescriptionNos) {
		auto presNoStr = std::string(presNo.getValue(""));
		if (presNoStr.empty()) continue;

		try {
			// 每张处方单独开启事务
			TransactionManager tm(&medReqDAO, &medDispDAO, &chargeDAO);

			// ---- 2a. 查询该处方下【有效状态】的用药申请记录 ----
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

			// ---- 2b. 校验发药记录：必须为【待调配PREPARATION】且未逻辑删除 ----
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

			// ---- 2c. 更新medication_request：状态修改为草稿 DRAFT ----
			for (auto& req : requests) {
				req->setStatusEnum("DRAFT");
				// rx_type_code 保持 "TCM" 不改变
				req->setUpdatedBy(currentUser);
				req->setUpdatedAt(now);
				medReqDAO.update(*req);
			}

			// ---- 2d. 更新medication_dispense：逻辑删除 is_deleted = 1 ----
			for (auto& d : dispensesToUpdate) {
				d->setIsDeleted("1");
				d->setUpdatedBy(currentUser);
				d->setUpdatedAt(now);
				medDispDAO.update(*d);
			}

			// ---- 2e. 更新adm_charge_item收费明细：逻辑删除 is_deleted = 1 ----
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

			// ---- 提交当前处方事务 ----
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

	// ---- 3. 组装返回结果 ----
	auto resultData = chinapres3RevokeResultDTO::createShared();
	resultData->revokedPrescriptionCount = revokedPresCount;
	resultData->revokedMedicationCount = revokedMedCount;
	resultData->failures = failureList;

	resultVo->success(resultData);
	resultVo->message = "Revoke completed";
	return resultVo;
}

// ===================================================================
//                         处方签发实现 Issue
// ===================================================================

/**
 * @brief 处方签发业务逻辑（草稿处方正式生效）
 * @param dto 处方签发入参DTO，就诊ID、患者ID、待签发药品条目列表
 * @param payload JWT身份载体，携带当前登录用户、租户ID
 * @return chinapres3IssueResultVO::Wrapper 签发结果VO，返回业务流水号
 * 业务规则：
 * 1. 仅【草稿DRAFT】状态处方允许签发
 * 2. 签发事务原子性：全部成功或全部回滚
 * 3. 执行流程：更新用药申请状态 → 生成发药记录 → 生成门诊收费项目
 */
chinapres3IssueResultVO::Wrapper chinapres3Service::issue(
	const chinapres3IssueDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto resultVo = chinapres3IssueResultVO::createShared();

	// ==================== 1. 参数校验 ====================
	auto encounterId = dto->encounterId;
	if (!encounterId || encounterId.getValue("") == "") {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass encounter ID";
		return resultVo;
	}
	// patientId removed from DTO: already present in med_medication_request, used directly
	auto signTargets = dto->signTargets;
	if (!signTargets || signTargets->size() == 0) {
		resultVo->code = 400;
		resultVo->message = "Parameter cannot be empty, please pass prescription list to sign";
		return resultVo;
	}

	// ==================== 2. 循环校验待签发处方条目 ====================
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
		// 校验就诊ID一致性
		auto reqEncounterId = req->getEncounterId();
		if (!reqEncounterId.empty() && reqEncounterId != encounterId.getValue("")) {
			resultVo->code = 400;
			resultVo->message = "Encounter ID mismatches prescription, prescription ID="
				+ medReqId.getValue("");
			return resultVo;
		}
		// patientId consistency no longer checked here:
		// patientId is derived from the medication request record,
		// and all sign targets must belong to the same encounter
		validRequests.push_back(req);
	}

	// ==================== 2b. All-or-nothing completeness check ====================
	// Group valid requests by prescription_no and verify every DRAFT drug line
	// under the same prescription is included in signTargets
	{
		std::map<std::string, v_int32> presCounts;
		for (auto& req : validRequests) {
			auto presNo = req->getPrescriptionNo();
			if (!presNo.empty()) {
				presCounts[presNo]++;
			}
		}
		for (auto& entry : presCounts) {
			uint64_t dbCount = medReqDAO.countByPrescriptionNo(entry.first, "DRAFT");
			if (static_cast<uint64_t>(entry.second) != dbCount) {
				std::ostringstream errMsg;
				errMsg << "Incomplete prescription: " << entry.first
					<< " has " << dbCount << " drug line(s) but only "
					<< entry.second << " selected. Please sign all or none.";
				resultVo->code = 400;
				resultVo->message = errMsg.str();
				return resultVo;
			}
		}
	}

	// ==================== 3. 事务内更新处方、生成发药记录与收费记录 ====================
	try
	{
		TransactionManager tm(&medReqDAO, &medDispDAO, &chargeDAO);
		auto now = SimpleDateTimeFormat::format();
		auto snowFlake = SnowFlake::getInstance();
		auto currentUser = payload.getUsername();
		auto orgId = payload.getId();

		std::vector<std::string> busNos;               // 存储生成的发药业务流水号
		std::set<std::string> processedPrescriptions;  // 去重：记录本次处理的处方号

		// ----- 3a. 更新处方状态为【已签发ACTIVE】 -----
		for (auto& req : validRequests) {
			req->setStatusEnum("ACTIVE");
			req->setRxTypeCode("TCM");
			req->setUpdatedBy(currentUser);
			req->setUpdatedAt(now);
			medReqDAO.update(*req);
		}

		// ----- 3b. 新增发药明细记录 medication_dispense -----
		// key:处方号, value:第一条用药申请ID，用于生成收费记录
		std::map<std::string, std::string> presNoToReqId;
		for (auto& req : validRequests) {
			chinapres3MedicationDispenseDO dispDO;
			dispDO.setId(std::to_string(snowFlake->nextId()));
			auto busNo = "BUS" + std::to_string(snowFlake->nextId());
			dispDO.setBusNo(busNo);
			busNos.push_back(busNo);
			dispDO.setStatusEnum("PREPARATION");        // 待调配状态
			dispDO.setDispenseEnum("OUTPATIENT");       // 门诊发药类型
			dispDO.setMedReqId(req->getId());
			dispDO.setPatientId(req->getPatientId());
			dispDO.setEncounterId(encounterId.getValue(""));
			// 从用药申请复制药品相关必填信息
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
				// 每个处方仅记录第一条用药申请，用于收费生成
				if (presNoToReqId.find(presNo) == presNoToReqId.end()) {
					presNoToReqId[presNo] = req->getId();
				}
			}
		}

		// ----- 3c. 生成门诊收费项目 adm_charge_item -----
		for (auto& presNo : processedPrescriptions) {
			chinapres3ChargeItemDO newCharge;
			newCharge.setId(std::to_string(snowFlake->nextId()));
			newCharge.setServiceTable("med_medication_request");
			newCharge.setServiceId(presNoToReqId[presNo]);
			newCharge.setPrescriptionNo(presNo);
			newCharge.setPatientId(validRequests.front()->getPatientId());
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

		// ----- 3d. 提交数据库事务 -----
		tm.commit();

		// ==================== 4. Assemble result ====================
	auto resultData = chinapres3IssueResultDTO::createShared();
	resultData->processedCount = static_cast<oatpp::Int32>(validRequests.size());
	resultData->prescriptionCount = static_cast<oatpp::Int32>(processedPrescriptions.size());

	// Collect prescription numbers
	auto presNoList = oatpp::Vector<oatpp::String>::createShared();
	for (auto& presNo : processedPrescriptions) {
		presNoList->push_back(oatpp::String(presNo.c_str()));
	}
	resultData->prescriptionNos = presNoList;
	resultData->statusEnum = "ACTIVE";
	resultData->issuedAt = now.c_str();

	// Dispense business serial numbers (bus_no for pharmacy lookup)
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