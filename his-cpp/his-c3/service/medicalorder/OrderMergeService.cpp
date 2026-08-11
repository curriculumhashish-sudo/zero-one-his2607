#include "stdafx.h"
#include "OrderMergeService.h"
#include "dao/medicalorder/MedOrderDAO.h"
#include "dao/medicalorder/AdmEncounterDAO.h"
#include <ctime>

MergeResultDTO::Wrapper OrderMergeService::mergeOrders(const OrderMergeDTO::Wrapper& dto)
{
	MergeResultDTO::Wrapper result = std::make_shared<MergeResultDTO>();

	//a)根据 encounterId 查询就诊记录，确保就诊存在
	//b)校验就诊记录中的患者ID与传入的 patientId 必须一致
	//c)校验就诊类型必须是住院（IMP）
	//d)校验就诊状态必须处于进行中（IN_PROGRESS）或暂停（ON_HOLD），只有这两种状态允许修改医嘱
	std::string encId = dto->encounterId.getValue("");
	std::string patId = dto->patientId.getValue("");
	if (encId.empty() || patId.empty()) {
		return nullptr;
	}
	if (!gateValidateEncounter(encId, patId, true)) {
		return nullptr;
	}

	//解析并校验操作人
	std::string operatorId = dto->operatorId.getValue("");
	if (operatorId.empty()) {
		return nullptr;
	}

	//校验是否包含有效操作
	bool hasClear = (dto->clearGroupIds && dto->clearGroupIds->size() > 0);
	bool hasAssign = (dto->assignGroup
		&& dto->assignGroup->medicationRequestIds
		&& dto->assignGroup->medicationRequestIds->size() > 0);
	if (!hasClear && !hasAssign) {
		return nullptr;
	}

	MedOrderDAO dao;
	std::string assignedGroupId;

	//执行拆组操作
	if (hasClear) {
		std::list<std::string> clearIds;
		//提取并过有效的医嘱id
		for (auto& id : *dto->clearGroupIds) {
			std::string idStr = id.getValue("");
			if (!idStr.empty()) {
				clearIds.push_back(idStr);
			}
		}
		if (!clearIds.empty()) {
			//逐条进行前置业务校验
			for (auto& id : clearIds) {
				auto order = dao.selectById(id);
				if (!order) {
					return nullptr;
				}
				if (order->getStatusEnum() == "STOPPED") {
					return nullptr;//已停嘱无拆嘱意义，拒绝操作
				}
			}
			//批量执行数据库更新
			int affected = dao.clearGroupIdBatch(clearIds);
			//校验：被影响的行数必须与传入的id数量一致，防止更新数据不一致
			if (affected != static_cast<int>(clearIds.size())) {
				return nullptr;
			}
		}
	}

	//执行并组操作
	//至少需要2条医嘱才能合并成组
	//所有参与并组的医嘱必须存在，且状态不能是已停嘱（STOPPED）或已取消（CANCELLED）
	//严禁跨患者并组（所有医嘱的 patientId 必须相同）
	if (hasAssign) {
		std::list<std::string> mergeIds;
		//提取并过滤有效的医嘱id
		for (auto& id : *dto->assignGroup->medicationRequestIds) {
			std::string idStr = id.getValue("");
			if (!idStr.empty()) {
				mergeIds.push_back(idStr);
			}
		}

		//至少两条才能并
		if (mergeIds.size() < 2) {
			return nullptr;
		}
		
		//获取前端传入的组号，如果未传入，则基于当前时间自动生成一个唯一组号
		assignedGroupId = dto->assignGroup->groupId.getValue("");
		if (assignedGroupId.empty()) {
			assignedGroupId = "G" + std::to_string(std::time(nullptr));
		}

		//逐条进行前置业务检验
		std::string firstPatId;
		bool first = true;
		for (auto& id : mergeIds) {
			auto order = dao.selectById(id);
			if (!order) {
				return nullptr;//医嘱不存在，操作失败
			}
		std::string status = order->getStatusEnum();
			//已停嘱或者已取消的医嘱不允许再被合并
			if (status == "STOPPED" || status == "CANCELLED") {
				return nullptr;
			}

			//跨患者并组拦截，以第一条医嘱的患者id为准，后续医嘱与其一致
			if (first) {
				firstPatId = order->getPatientId();
				first = false;
			}
			else {
				if (order->getPatientId() != firstPatId) {
					return nullptr;//发现跨患者并组，拒绝操作
				}
			}
		}

		//批量执行数据库更新
		int affected = dao.updateGroupIdBatch(mergeIds, assignedGroupId);
		//严格校验：受影响的行数必须与传入id数量一致
		if (affected != static_cast<int>(mergeIds.size())) {
			return nullptr;
		}

		result->groupId = assignedGroupId;
	}


	return result;
}

bool OrderMergeService::gateValidateEncounter(
	const std::string& encounterId,
	const std::string& patientId,
	bool writable)
{
	AdmEncounterDAO dao;
	auto enc = dao.selectById(encounterId);
	if (!enc) return false;
	if (enc->getPatientId() != patientId) return false;
	if (writable) {
		std::string s = enc->getStatusEnum();
		if (s != "IN_PROGRESS" && s != "ON_HOLD") return false;
	}
	return true;
}