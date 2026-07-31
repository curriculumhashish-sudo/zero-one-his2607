/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "stdafx.h"
#include "id/SnowFlake.h"
#include "id/UuidFacade.h"
#include "OrderExecuteControlService.h"

#include <list>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "dao/medicalorderexecution/OrderExecuteControlDAO.h"
#include "SimpleDateTimeFormat.h"

namespace
{
	std::string buildOriginalOrderMapKey(const std::string& adviceType, const std::string& adviceId)
	{
		return adviceType + "|" + adviceId;
	}

	std::string normalizeOccurrenceTimeToSecond(const std::string& occurrenceTime)
	{
		auto dotPos = occurrenceTime.find('.');
		if (dotPos == std::string::npos)
			return occurrenceTime;
		return occurrenceTime.substr(0, dotPos);
	}

	std::string buildProcedureSlotKey(const std::string& requestId, const std::string& requestTable, const std::string& occurrenceTime)
	{
		return requestId + "|" + requestTable + "|" + normalizeOccurrenceTimeToSecond(occurrenceTime);
	}

	void fillCancelProcedure(
		ClnProcedureDO& cancelProcedure,
		const PtrClnProcedureDO& completedProcedure,
		const std::string& cancelProcedureId,
		const std::string& nowTime,
		const std::string& operatorId,
		const std::string& statusReasonText)
	{
		cancelProcedure.setId(cancelProcedureId);
		if (completedProcedure->getTenantIdPtr()) cancelProcedure.setTenantId(completedProcedure->getTenantId());
		cancelProcedure.setCreatedAt(nowTime);
		cancelProcedure.setCreatedBy(operatorId);
		cancelProcedure.setIsDeleted(false);
		cancelProcedure.setVersion(0);
		cancelProcedure.setRequestId(completedProcedure->getRequestId());
		cancelProcedure.setRequestTable(completedProcedure->getRequestTable());
		cancelProcedure.setStatusEnum("CANCEL");
		cancelProcedure.setStatusReasonText(statusReasonText);
		cancelProcedure.setCategoryEnum(completedProcedure->getCategoryEnum());
		cancelProcedure.setEncounterId(completedProcedure->getEncounterId());
		cancelProcedure.setPatientId(completedProcedure->getPatientId());
		if (completedProcedure->getOccurrenceTimePtr()) cancelProcedure.setOccurrenceTime(completedProcedure->getOccurrenceTime());
		cancelProcedure.setRecordedTime(nowTime);
		cancelProcedure.setRecorderId(operatorId);
		if (completedProcedure->getOrgIdPtr()) cancelProcedure.setOrgId(completedProcedure->getOrgId());
		if (completedProcedure->getLocationIdPtr()) cancelProcedure.setLocationId(completedProcedure->getLocationId());
		if (completedProcedure->getGroupIdPtr()) cancelProcedure.setGroupId(completedProcedure->getGroupId());
		cancelProcedure.setRefundId(completedProcedure->getId());
		cancelProcedure.setOccurrenceSlotActive(false);
	}

	bool appendUniqueId(
		std::list<std::string>& ids,
		std::unordered_set<std::string>& idSet,
		const std::string& id)
	{
		if (id.empty())
			return false;
		if (!idSet.insert(id).second)
			return false;
		ids.push_back(id);
		return true;
	}

	void copyOptionalMedicationDispenseFields(
		MedicationDispenseDO& target,
		const PtrMedicationDispenseDO& source)
	{
		if (source->getTenantIdPtr()) target.setTenantId(source->getTenantId());
		if (source->getPlannedDispenseTimePtr()) target.setPlannedDispenseTime(source->getPlannedDispenseTime());
		if (source->getDesLocationIdPtr()) target.setDesLocationId(source->getDesLocationId());
		if (source->getDosageInstructionPtr()) target.setDosageInstruction(source->getDosageInstruction());
		if (source->getMethodCodePtr()) target.setMethodCode(source->getMethodCode());
		if (source->getFrequencyCodePtr()) target.setFrequencyCode(source->getFrequencyCode());
		if (source->getDosePtr()) target.setDose(source->getDose());
		if (source->getDoseUnitCodePtr()) target.setDoseUnitCode(source->getDoseUnitCode());
		if (source->getMaxUnitPtr()) target.setMaxUnit(source->getMaxUnit());
		if (source->getPartTypeCodePtr()) target.setPartTypeCode(source->getPartTypeCode());
		if (source->getDispenseFrequencyCodePtr()) target.setDispenseFrequencyCode(source->getDispenseFrequencyCode());
	}

	MedicationDispenseDO buildMedicationRefundDispense(
		const PtrMedicationDispenseDO& completedDispense,
		const std::string& refundDispenseId,
		const std::string& cancelProcedureId,
		const std::string& nowTime,
		const std::string& operatorId)
	{
		MedicationDispenseDO refundDispense;
		refundDispense.setId(refundDispenseId);
		copyOptionalMedicationDispenseFields(refundDispense, completedDispense);
		refundDispense.setCreatedAt(nowTime);
		refundDispense.setCreatedBy(operatorId);
		refundDispense.setIsDeleted(false);
		refundDispense.setVersion(0);
		refundDispense.setBusNo("RF" + refundDispenseId);
		refundDispense.setStatusEnum("PENDING_REFUND");
		refundDispense.setStatusChangedTime(nowTime);
		refundDispense.setDispenseEnum("REFUND");
		refundDispense.setMedicationId(completedDispense->getMedicationId());
		refundDispense.setPatientId(completedDispense->getPatientId());
		refundDispense.setEncounterId(completedDispense->getEncounterId());
		refundDispense.setLocationId(completedDispense->getLocationId());
		refundDispense.setMedReqId(completedDispense->getMedReqId());
		refundDispense.setQuantity(
			completedDispense->getDispenseQuantityPtr()
				? completedDispense->getDispenseQuantity()
				: completedDispense->getQuantity());
		refundDispense.setUnitCode(completedDispense->getUnitCode());
		refundDispense.setProcedureId(cancelProcedureId);
		refundDispense.setRecPractitionerId(operatorId);
		return refundDispense;
	}

	SupplyRequestDO buildMedicationSummarySupplyRequest(
		const MedicationSummaryLineRow& line,
		const std::string& requestId,
		const std::string& summaryNo,
		const std::string& nowTime,
		const std::string& operatorId)
	{
		SupplyRequestDO request;
		request.setId(requestId);
		request.setCreatedAt(nowTime);
		request.setCreatedBy(operatorId);
		request.setIsDeleted(false);
		request.setVersion(0);
		request.setBusNo(summaryNo);
		request.setTypeEnum("27");
		request.setStatusEnum("PENDING_APPROVAL");
		request.setCategoryEnum("MEDICATION");
		request.setPatientId(line.patientId);
		request.setItemTable("med_medication_definition");
		request.setItemQuantity(line.quantity);
		request.setItemId(line.medicationId);
		request.setUnitCode(line.unitCode);
		request.setPurposeTypeEnum("LOCATION");
		request.setPurposeLocationId(line.locationId);
		request.setApplicantId(operatorId);
		request.setPrice(line.price);
		request.setTotalPrice(line.price * line.quantity);
		return request;
	}
}

//已加当前操作者权限范围校验，为了避免"越权登记别的病区医嘱为“不执行”"
std::string OrderExecuteControlService::skipOrderExecution(const oatpp::List<OrderNotExecuteDTO::Wrapper>& dtolist, const PayloadDTO& payload)
{
	//基础校验，service可能被其他模块直接调用
	if (!dtolist || dtolist->empty() || dtolist->size() > 50)
		return "ORDER_NOT_EXECUTE_LIST_INVALID";
	for (auto& dto : *dtolist){
		auto checkinfo = OrderNotExecuteDTO::validate(dto);
		if (!checkinfo.empty())
			return checkinfo;
	}
	//检查是否存在重复请求
	std::unordered_set<std::string> requestKeys;
	for (auto& dto : *dtolist){
		if (!requestKeys.insert(buildNotExecuteRequestKey(dto)).second)
			return "ORDER_NOT_EXECUTE_DUPLICATE_ITEM";
	}

	std::list<std::string> medOrderIds;
	std::list<std::string> serviceOrderIds;
	std::list<std::tuple<std::string, std::string, std::string>> slots;
	std::unordered_set<std::string> medOrderIdSet;
	std::unordered_set<std::string> serviceOrderIdSet;
	std::unordered_set<std::string> slotKeys;
	for (auto& dto : *dtolist)
	{
		string requestTable;
		if (*dto->advice_type.getPtr() == "MED_REQUEST")
		{
			requestTable = "med_medication_request";
			if (medOrderIdSet.insert(*dto->advice_id.getPtr()).second)
				medOrderIds.push_back(*dto->advice_id.getPtr());
		}
		else if (*dto->advice_type.getPtr() == "SERVICE_REQUEST")
		{
			requestTable = "wkf_service_request";
			if (serviceOrderIdSet.insert(*dto->advice_id.getPtr()).second)
				serviceOrderIds.push_back(*dto->advice_id.getPtr());
		}
		else
			return "ORDER_NOT_EXECUTE_ADVICE_TYPE_INVALID";

		const auto occurrenceTime = normalizeOccurrenceTimeToSecond(*dto->plan_time.getPtr());
		auto slotKey = buildProcedureSlotKey(
			*dto->advice_id.getPtr(),
			requestTable,
			occurrenceTime);
		if (!slotKeys.insert(slotKey).second)
			return "ORDER_NOT_EXECUTE_DUPLICATE_ITEM";//重复请求校验，存在重复请求直接决绝

		slots.emplace_back(
			*dto->advice_id.getPtr(),
			requestTable,
			occurrenceTime);
	}

	OrderExecuteControlDAO dao;
	auto operableEncounterIds = listOperableEncounterIds(payload); //查询当前护士可操作的encounter_id集合，后续进行权限校验
	if (operableEncounterIds.empty())
		return "ORDER_NOT_EXECUTE_OPERATOR_SCOPE_EMPTY";

	//开启事务，只允许批量成功/失败
	dao.getSqlSession()->beginTransaction();
	try
	{
		std::unordered_map<std::string, PtrOriginalOrderRequestDO> originalOrderMap;
		//批量查询原始医嘱，因为来源表有两张，所以下面调用了两次查询函数
		for (const auto& item : dao.queryOriginalOrderRequestById(medOrderIds, "MED_REQUEST"))
		{
			originalOrderMap[buildOriginalOrderMapKey("MED_REQUEST", item.first)] = item.second; //构建 "[原始医嘱表, 医嘱id] -> 后续关键业务字段的映射"
		}
		for (const auto& item : dao.queryOriginalOrderRequestById(serviceOrderIds, "SERVICE_REQUEST"))
		{
			originalOrderMap[buildOriginalOrderMapKey("SERVICE_REQUEST", item.first)] = item.second;
		}

		for (auto& dto : *dtolist)
		{
			string requestTable;
			if (*dto->advice_type.getPtr() == "MED_REQUEST")
				requestTable = "med_medication_request";
			else
				requestTable = "wkf_service_request";

			auto originalOrderIt = originalOrderMap.find(buildOriginalOrderMapKey(*dto->advice_type.getPtr(), *dto->advice_id.getPtr()));
			if (originalOrderIt == originalOrderMap.end() || !originalOrderIt->second) { //这条不执行请求没有查询对应医嘱(map里找到dto中的key)
				dao.getSqlSession()->rollbackTransaction();
				return "ORDER_NOT_EXECUTE_ORIGINAL_ORDER_NOT_FOUND";
			}
			auto originalOrder = originalOrderIt->second;
			if (!canOperateEncounter(operableEncounterIds, originalOrder->getEncounterId())) //权限校验：如果根据原医嘱查询出来的encounter_id不在可操作encounter_id中，拒绝
			{
				dao.getSqlSession()->rollbackTransaction();
				return "ORDER_NOT_EXECUTE_ENCOUNTER_NOT_OPERABLE";
			}
		}

		/*pending净态校验，查询执行记录表是否存在同id已执行或者不执行*/
		if (dao.existsActiveTerminalProcedureBySlot(slots))
		{
			dao.getSqlSession()->rollbackTransaction();
			return "ORDER_NOT_EXECUTE_SLOT_ALREADY_OCCUPIED";
		}

		for (auto& dto : *dtolist)
		{
			string requestTable;
			if (*dto->advice_type.getPtr() == "MED_REQUEST")
				requestTable = "med_medication_request";
			else
				requestTable = "wkf_service_request";

			auto originalOrder = originalOrderMap.find(buildOriginalOrderMapKey(*dto->advice_type.getPtr(), *dto->advice_id.getPtr()))->second;
			/*生成雪花id*/
			string procedureId = std::to_string(SnowFlake::getInstance()->nextId());
			if (procedureId == "0")
			{
				dao.getSqlSession()->rollbackTransaction();
				return "ORDER_NOT_EXECUTE_SYSTEM_ERROR";
			}

			string nowTime = SimpleDateTimeFormat::format();
			const auto occurrenceTime = normalizeOccurrenceTimeToSecond(*dto->plan_time.getPtr());
			
			/*设置要插入的一行的关键字段*/
			ClnProcedureDO procedure;
			procedure.setNotExecuteInsertFields(
				procedureId,
				nowTime,
				payload.getId(),
				*dto->advice_id.getPtr(),
				requestTable,
				originalOrder->getCategoryEnum(),
				originalOrder->getEncounterId(),
				originalOrder->getPatientId(),
				occurrenceTime);

			/*一条插入失败就整批事务回滚*/
			if (dao.insert(procedure) != 1)
			{
				dao.getSqlSession()->rollbackTransaction();
				return "ORDER_NOT_EXECUTE_INSERT_PROCEDURE_FAILED";
			}
		}

		/*提交事务*/
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		/*捕获异常后先回滚事务*/
		dao.getSqlSession()->rollbackTransaction();
		return "ORDER_NOT_EXECUTE_DATABASE_ERROR";
	}
	return ""; //成功返回空字符串
}








CancelExecutionResultDTO::Wrapper OrderExecuteControlService::cancelOrderExecution(
	const CancelExecutionRequestDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto result = CancelExecutionResultDTO::createShared();
	result->rebuiltSummaryNos = oatpp::List<oatpp::String>::createShared();

	//基础参数校验
	if (!dto)
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_REQUEST_REQUIRED";
		return result;
	}
	auto checkinfo = dto->validate();
	if (!checkinfo.empty())
	{
		result->success = false;
		result->message = checkinfo;
		return result;
	}

	//校验不存在重复请求
	std::list<std::string> procedureIdList;
	std::unordered_set<std::string> procedureIds;
	for (auto& item : *dto->items)
	{
		if (!procedureIds.insert(*item->procedureId.getPtr()).second)
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_DUPLICATE_PROCEDURE";
			return result;
		}
		procedureIdList.push_back(*item->procedureId.getPtr());
	}

	//获取当前护士可操作encounterids集合，以便后续权限校验
	auto operableEncounterIds = listOperableEncounterIds(payload);
	if (operableEncounterIds.empty())
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_OPERATOR_SCOPE_EMPTY";
		return result;
	}

	//校验不存在 "已指向待取消的已执行记录" 的重复取消记录(防止重复取消)
	OrderExecuteControlDAO dao;
	if (dao.existsCancelProcedureByRefundIds(procedureIdList))
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_ALREADY_CANCELLED";
		return result;
	}

	std::unordered_map<std::string, PtrClnProcedureDO> procedureMap;
	for (const auto& procedure : dao.queryCompletedProcedureById(procedureIdList))
			procedureMap[procedure->getId()] = procedure;

	for (auto& item : *dto->items)
	{
		const std::string procedureId = *item->procedureId.getPtr();
		const std::string requestEncounterId = *item->encounterId.getPtr();
		auto procedureIt = procedureMap.find(procedureId);
		if (procedureIt == procedureMap.end()) //如果没查到原已执行记录，拒绝
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_PROCEDURE_NOT_FOUND";
			return result;
		}
		auto procedure = procedureIt->second;
		if (procedure->getEncounterId() != requestEncounterId) //如果查询出的encounterid不等于前端传过来的encounterid(可见encounterid操作不可见procedure)，拒绝
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_ENCOUNTER_MISMATCH";
			return result;
		}
		if (!canOperateEncounter(operableEncounterIds, requestEncounterId)) //校验当前护士是否能操作当前患者
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_ENCOUNTER_NOT_OPERABLE";
			return result;
		}
	}

	std::list<std::string> allProcedureIdList = procedureIdList;
	std::unordered_set<std::string> allProcedureIds = procedureIds;
	std::list<std::string> derivedProcedureIdList;
	auto derivedServiceRequestIds = dao.listDerivedServiceRequestIdsForCancellation(procedureIdList);
	for (const auto& procedure : dao.queryCompletedProceduresByServiceRequestIds(derivedServiceRequestIds))
	{
		if (!procedure || !appendUniqueId(allProcedureIdList, allProcedureIds, procedure->getId()))
			continue;
		derivedProcedureIdList.push_back(procedure->getId());
		procedureMap[procedure->getId()] = procedure;
		if (!canOperateEncounter(operableEncounterIds, procedure->getEncounterId()))
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_DERIVED_ENCOUNTER_NOT_OPERABLE";
			return result;
		}
	}

	if (!derivedProcedureIdList.empty() && dao.existsCancelProcedureByRefundIds(derivedProcedureIdList))
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_DERIVED_ALREADY_CANCELLED";
		return result;
	}

	if (dao.existsBlockingChargeItemsForCancellation(allProcedureIdList, derivedServiceRequestIds))
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_CHARGE_NOT_ROLLBACKABLE";
		return result;
	}
	if (dao.existsBlockingMedicationDispensesForCancellation(allProcedureIdList))
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_MEDICATION_DISPENSE_NOT_ROLLBACKABLE";
		return result;
	}
	auto medicationSummaryNos = dao.listMedicationSummaryNosByProcedureIds(allProcedureIdList);
	if (dao.existsBlockingMedicationSummaryByNos(medicationSummaryNos))
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_MEDICATION_SUMMARY_NOT_ROLLBACKABLE";
		return result;
	}
	if (dao.existsBlockingDeviceDispensesForCancellation(allProcedureIdList))
	{
		result->success = false;
		result->message = "CANCEL_EXECUTION_DEVICE_DISPENSE_NOT_ROLLBACKABLE";
		return result;
	}

	std::list<ClnProcedureDO> cancelProcedures;
	std::unordered_map<std::string, std::string> cancelProcedureIdByOriginalId;
	const std::string nowTime = SimpleDateTimeFormat::format();
	const std::string statusReasonText = *dto->statusReasonText.getPtr();
	for (const auto& procedureId : allProcedureIdList)
	{
		const std::string cancelProcedureId = std::to_string(SnowFlake::getInstance()->nextId());
		if (cancelProcedureId == "0")
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_SYSTEM_ERROR";
			return result;
		}
		cancelProcedureIdByOriginalId[procedureId] = cancelProcedureId;
		cancelProcedures.emplace_back();
		fillCancelProcedure(
			cancelProcedures.back(),
			procedureMap.find(procedureId)->second,
			cancelProcedureId,
			nowTime,
			payload.getId(),
			statusReasonText);
	}

	std::list<MedicationDispenseDO> refundMedicationDispenses;
	for (const auto& completedDispense : dao.queryCompletedMedicationDispensesForRefund(allProcedureIdList))
	{
		if (!completedDispense || !completedDispense->getProcedureIdPtr())
			continue;
		auto cancelProcedureIt = cancelProcedureIdByOriginalId.find(completedDispense->getProcedureId());
		if (cancelProcedureIt == cancelProcedureIdByOriginalId.end())
			continue;
		const std::string refundDispenseId = std::to_string(SnowFlake::getInstance()->nextId());
		if (refundDispenseId == "0")
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_SYSTEM_ERROR";
			return result;
		}
		refundMedicationDispenses.push_back(buildMedicationRefundDispense(
			completedDispense,
			refundDispenseId,
			cancelProcedureIt->second,
			nowTime,
			payload.getId()));
	}

	dao.getSqlSession()->beginTransaction(); //开启事务，按数据库更改动作类别批量执行更改
	try
	{
		// 先批量置0原先已执行记录占用计划点标志
		if (dao.releaseCompletedProcedureSlotsNoTransaction(allProcedureIdList) != static_cast<int>(allProcedureIdList.size()))
		{
			dao.getSqlSession()->rollbackTransaction();
			result->success = false;
			result->message = "CANCEL_EXECUTION_RELEASE_SLOT_FAILED";
			return result;
		}

		// 再批量插入取消执行对冲cancel行
		if (dao.insertCancelProceduresNoTransaction(cancelProcedures) != static_cast<int>(cancelProcedures.size()))
		{
			dao.getSqlSession()->rollbackTransaction();
			result->success = false;
			result->message = "CANCEL_EXECUTION_INSERT_CANCEL_FAILED";
			return result;
		}

		dao.voidPendingChargeItemsForCancellationNoTransaction(allProcedureIdList, derivedServiceRequestIds, payload.getId());
		dao.softDeleteMedicationDispensesForCancellationNoTransaction(allProcedureIdList, payload.getId());
		if (!refundMedicationDispenses.empty()
			&& dao.insertMedicationRefundDispensesNoTransaction(refundMedicationDispenses)
				!= static_cast<int>(refundMedicationDispenses.size()))
		{
			dao.getSqlSession()->rollbackTransaction();
			result->success = false;
			result->message = "CANCEL_EXECUTION_INSERT_REFUND_DISPENSE_FAILED";
			return result;
		}

		dao.softDeleteMedicationSummarySupplyRequestsNoTransaction(medicationSummaryNos, payload.getId());
		dao.clearMedicationSummaryNoForCancellationNoTransaction(allProcedureIdList, payload.getId());
		UuidFacade uuidFacade;
		for (const auto& summaryNo : medicationSummaryNos)
		{
			auto summaryLines = dao.queryMedicationSummaryLinesForRebuild(summaryNo);
			if (summaryLines.empty())
				continue;

			const std::string rebuiltSummaryNo = "MS" + uuidFacade.genUuid();
			if (dao.reassignMedicationSummaryNoTransaction(summaryNo, rebuiltSummaryNo, payload.getId())
				!= static_cast<int>(summaryLines.size()))
			{
				dao.getSqlSession()->rollbackTransaction();
				result->success = false;
				result->message = "CANCEL_EXECUTION_SUMMARY_REBUILD_FAILED";
				return result;
			}

			std::list<SupplyRequestDO> rebuiltSupplyRequests;
			for (const auto& line : summaryLines)
			{
				rebuiltSupplyRequests.push_back(buildMedicationSummarySupplyRequest(
					line,
					uuidFacade.genUuid(),
					rebuiltSummaryNo,
					nowTime,
					payload.getId()));
			}
			if (dao.insertMedicationSummarySupplyRequestsNoTransaction(rebuiltSupplyRequests)
				!= static_cast<int>(rebuiltSupplyRequests.size()))
			{
				dao.getSqlSession()->rollbackTransaction();
				result->success = false;
				result->message = "CANCEL_EXECUTION_SUMMARY_REBUILD_FAILED";
				return result;
			}
			result->rebuiltSummaryNos->push_back(rebuiltSummaryNo);
		}

		dao.restoreInventoryForCompletedDeviceDispensesNoTransaction(allProcedureIdList, payload.getId());
		dao.softDeleteDeviceDispensesForCancellationNoTransaction(allProcedureIdList, payload.getId());
		dao.softDeleteDerivedServiceRequestDetailsNoTransaction(derivedServiceRequestIds, payload.getId());
		dao.softDeleteDerivedServiceRequestsNoTransaction(derivedServiceRequestIds, payload.getId());

		dao.getSqlSession()->commitTransaction();
		result->success = true;
		result->message = "success";
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
		result->success = false;
		result->message = "CANCEL_EXECUTION_DATABASE_ERROR";
	}
	return result;
}









std::unordered_set<std::string> OrderExecuteControlService::listOperableEncounterIds(const PayloadDTO& payload) const
{ //以unordered_set的方式返回所有可操作encounter_id
	std::unordered_set<std::string> result;
	if (payload.getId().empty())
		return result;

	OrderExecuteControlDAO dao;
	auto encounterIds = dao.listOperableEncounterIdsByUserId(payload.getId());
	for (const auto& encounterId : encounterIds)
	{
		if (!encounterId.empty())
			result.insert(encounterId);
	}
	return result;
}

bool OrderExecuteControlService::canOperateEncounter(
	const std::unordered_set<std::string>& operableEncounterIds,
	const std::string& encounterId) const
{
	if (encounterId.empty()) //如果没有可操作encounter_id
		return false;
	return operableEncounterIds.find(encounterId) != operableEncounterIds.end(); //返回是否能在可操作住院记录集合中找到对应encounter_id
}

std::string OrderExecuteControlService::buildNotExecuteRequestKey(const OrderNotExecuteDTO::Wrapper& dto) const
{
	return *dto->advice_id.getPtr() + "|" + *dto->advice_type.getPtr() + "|" + normalizeOccurrenceTimeToSecond(*dto->plan_time.getPtr());
}
