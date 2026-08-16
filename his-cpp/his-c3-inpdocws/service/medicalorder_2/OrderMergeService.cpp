#include "stdafx.h"
#pragma execution_character_set("utf-8")
#include "OrderMergeService.h"
#include "dao/medicalorder_2/MedOrderDAO.h"
#include "dao/medicalorder_2/AdmEncounterDAO.h"
#include <ctime>

MergeResultDTO::Wrapper OrderMergeService::mergeOrders(const OrderMergeDTO::Wrapper& dto)
{
	MergeResultDTO::Wrapper result = std::make_shared<MergeResultDTO>();

	// Gate1: Validate encounter
	std::string encId = dto->encounterId.getValue("");
	std::string patId = dto->patientId.getValue("");
	if (encId.empty() || patId.empty()) {
		auto err = MergeResultDTO::createShared();
		err->errorMessage = "Missing encounter ID or patient ID";
		return err;
	}
	if (!gateValidateEncounter(encId, patId, true)) {
		auto err = MergeResultDTO::createShared();
		err->errorMessage = "Encounter validation failed";
		return err;
	}

	// Gate2: Validate operator
	std::string operatorId = dto->operatorId.getValue("");
	if (operatorId.empty()) {
		auto err = MergeResultDTO::createShared();
		err->errorMessage = "Missing operator ID";
		return err;
	}

	// Gate3: Validate effective operations
	bool hasClear = (dto->clearGroupIds && dto->clearGroupIds->size() > 0);
	bool hasAssign = (dto->assignGroup
		&& dto->assignGroup->medicationRequestIds
		&& dto->assignGroup->medicationRequestIds->size() > 0);
	if (!hasClear && !hasAssign) {
		auto err = MergeResultDTO::createShared();
		err->errorMessage = "Missing valid operation parameters";
		return err;
	}

	MedOrderDAO dao;
	std::string assignedGroupId;

	// ---- Execute clear group operation ----
	if (hasClear) {
		std::list<std::string> clearIds;
		for (auto& id : *dto->clearGroupIds) {
			std::string idStr = id.getValue("");
			if (!idStr.empty()) {
				clearIds.push_back(idStr);
			}
		}
		if (!clearIds.empty()) {
			for (auto& id : clearIds) {
				auto order = dao.selectById(id);
				if (!order) {
					auto err = MergeResultDTO::createShared();
					err->errorMessage = "Order " + id + " not found";
					return err;
				}
				if (order->getStatusEnum() == "STOPPED") {
					auto err = MergeResultDTO::createShared();
					err->errorMessage = "Order " + id + " already stopped, cannot clear group";
					return err;
				}
				if (order->getEncounterId() != encId || order->getPatientId() != patId) {
					auto err = MergeResultDTO::createShared();
					err->errorMessage = "Order " + id + " does not belong to current encounter or patient";
					return err;
				}
			}
			try {
				dao.beginTransaction();
				int affected = dao.clearGroupIdBatch(clearIds, encId, patId);
				if (affected != static_cast<int>(clearIds.size())) {
					dao.rollbackTransaction();
					auto err = MergeResultDTO::createShared();
					err->errorMessage = "Clear group operation incomplete";
					return err;
				}
				dao.commitTransaction();
			} catch (...) {
				dao.rollbackTransaction();
				auto err = MergeResultDTO::createShared();
				err->errorMessage = "Clear group operation exception";
				return err;
			}
		}
	}

	// ---- Execute merge group operation ----
	if (hasAssign) {
		std::list<std::string> mergeIds;
		for (auto& id : *dto->assignGroup->medicationRequestIds) {
			std::string idStr = id.getValue("");
			if (!idStr.empty()) {
				mergeIds.push_back(idStr);
			}
		}

		if (mergeIds.size() < 2) {
			auto err = MergeResultDTO::createShared();
			err->errorMessage = "Merge group operation requires at least 2 orders";
			return err;
		}

		assignedGroupId = dto->assignGroup->groupId.getValue("");
		if (assignedGroupId.empty()) {
			assignedGroupId = "G" + std::to_string(std::time(nullptr));
		}

		std::string firstPatId;
		bool first = true;
		for (auto& id : mergeIds) {
			auto order = dao.selectById(id);
			if (!order) {
				auto err = MergeResultDTO::createShared();
				err->errorMessage = "Order " + id + " not found";
				return err;
			}
			std::string status = order->getStatusEnum();
			if (status == "STOPPED" || status == "CANCELLED") {
				auto err = MergeResultDTO::createShared();
				err->errorMessage = "Order " + id + " already stopped or cancelled";
				return err;
			}
			if (order->getEncounterId() != encId || order->getPatientId() != patId) {
				auto err = MergeResultDTO::createShared();
				err->errorMessage = "Order " + id + " does not belong to current encounter or patient";
				return err;
			}
			if (first) {
				firstPatId = order->getPatientId();
				first = false;
			} else {
				if (order->getPatientId() != firstPatId) {
					auto err = MergeResultDTO::createShared();
					err->errorMessage = "Orders belong to different patients, merge rejected";
					return err;
				}
			}
		}

		try {
			dao.beginTransaction();
			int affected = dao.updateGroupIdBatch(mergeIds, assignedGroupId, encId, patId);
			if (affected != static_cast<int>(mergeIds.size())) {
				dao.rollbackTransaction();
				auto err = MergeResultDTO::createShared();
				err->errorMessage = "Merge group operation incomplete";
				return err;
			}
			dao.commitTransaction();
		} catch (...) {
			dao.rollbackTransaction();
			auto err = MergeResultDTO::createShared();
			err->errorMessage = "Merge group operation exception";
			return err;
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
