/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:26:52

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
#include "CycleControlService.h"
#include "dao/cycle_control/PatientQueueDAO.h"
#include "dao/cycle_control/DischargeOrderDAO.h"
#include "TransactionManager.h"

PatientQueuePageDTO::Wrapper CycleControlService::queryPatientQueue(const PatientQueueQuery::Wrapper& query)
{
	auto pages = PatientQueuePageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	PatientQueueDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0) return pages;

	pages->total = count;
	pages->calcPages();

	list<PatientQueueDO> result = dao.selectWithPage(query);
	for (PatientQueueDO& item : result)
	{
		auto dto = PatientQueueDTO::createShared();
		dto->encounterId = item.getEncounterId();
		dto->patientId = item.getPatientId();
		dto->patientName = item.getPatientName();
		dto->gender = item.getGender();
		dto->age = item.getAge();
		dto->diagnosisSummary = item.getDiagnosisSummary();
		dto->bedName = item.getBedName();
		dto->bedId = item.getBedId();
		dto->wardId = item.getWardId();
		dto->wardName = item.getWardName();
		dto->houseName = item.getHouseName();
		dto->accountBalance = item.getAccountBalance();
		dto->statusEnum = item.getStatusEnum();
		dto->encounterBusNo = item.getEncounterBusNo();
		dto->admitTime = item.getAdmitTime();
		dto->organizationId = item.getOrganizationId();
		dto->organizationName = item.getOrganizationName();
		pages->addData(dto);
	}
	return pages;
}

DischargeOrderDetailDTO::Wrapper CycleControlService::createDischargeOrder(const CreateDischargeOrderDTO::Wrapper& dto)
{
	string encounterId = dto->encounterId.getValue("");

	DischargeOrderDAO dao;
	// 幂等判断：已存在有效出院医嘱时直接返回已有结果
	string existOrderId = dao.queryActiveDischargeOrder(encounterId);
	if (!existOrderId.empty())
	{
		auto result = DischargeOrderDetailDTO::createShared();
		result->orderId = existOrderId;
		result->encounterId = encounterId;
		result->statusEnum = "ACTIVE";
		result->encounterNewStatus = "ON_HOLD";
		return result;
	}
	// 校验就诊存在且为在院状态
	if (!dao.encounterInProgress(encounterId))
	{
		return nullptr;
	}

	// 事务内完成插入医嘱与更新就诊状态
	TransactionManager tm(&dao);
	string orderId = dao.insertDischargeOrder(dto);
	int rows = dao.updateEncounterStatus(encounterId);
	if (orderId.empty() || rows != 1)
	{
		tm.rollback();
		return nullptr;
	}
	tm.commit();

	auto result = DischargeOrderDetailDTO::createShared();
	result->orderId = orderId;
	result->encounterId = encounterId;
	result->statusEnum = "ACTIVE";
	result->encounterNewStatus = "ON_HOLD";
	return result;
}
