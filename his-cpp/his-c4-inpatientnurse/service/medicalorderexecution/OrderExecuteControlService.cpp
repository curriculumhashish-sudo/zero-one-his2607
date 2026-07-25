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

	// TODO: Call Charge service gate to reject cancellation when related charges are settled, refunding, or refunded.
	// TODO: Call medication dispense/refund service gate to reject cancellation when drugs are prepared or dispensing.
	// TODO: Call medication summary service gate to reject cancellation when related summary orders cannot be revoked or rebuilt.
	// TODO: Call material dispense/inventory service gate to reject cancellation when related materials cannot be rolled back.
	// TODO: Call derived service request service gate to reject cancellation when bound diagnosis/treatment items cannot be rolled back.

	std::list<ClnProcedureDO> cancelProcedures;
	const std::string nowTime = SimpleDateTimeFormat::format();
	const std::string statusReasonText = *dto->statusReasonText.getPtr();
	for (auto& item : *dto->items)
	{
		const std::string procedureId = *item->procedureId.getPtr();
		const std::string cancelProcedureId = std::to_string(SnowFlake::getInstance()->nextId());
		if (cancelProcedureId == "0")
		{
			result->success = false;
			result->message = "CANCEL_EXECUTION_SYSTEM_ERROR";
			return result;
		}
		cancelProcedures.emplace_back();
		fillCancelProcedure(
			cancelProcedures.back(),
			procedureMap.find(procedureId)->second,
			cancelProcedureId,
			nowTime,
			payload.getId(),
			statusReasonText);
	}

	dao.getSqlSession()->beginTransaction(); //开启事务，按数据库更改动作类别批量执行更改
	try
	{
		// 先批量置0原先已执行记录占用计划点标志
		if (dao.releaseCompletedProcedureSlotsNoTransaction(procedureIdList) != static_cast<int>(procedureIdList.size()))
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

		// TODO: Call Charge service to void or reverse pending-settlement charge items generated by these procedures.
		// TODO: Call medication dispense/refund service to remove unissued dispense rows or create pending drug return orders.
		// TODO: Call medication summary service to revoke affected summaries, rebuild remaining summaries, and return rebuilt summary numbers.
		// TODO: Call material dispense/inventory service to cancel material dispense rows and restore inventory when needed.
		// TODO: Call derived service request service to remove bound derived requests, cancel derived procedures, and trigger charge rollback.

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
