#include "stdafx.h"
#include "MedicalOrderService.h"
#include "../../dao/medicalorder/MedicationDAO.h"
#include "../../dao/medicalorder/ServiceRequestDAO.h"

// ==================== 医嘱状态常量 ====================
// 待签发：医生已开立但未确认，不可执行
#define STATUS_DRAFT     "DRAFT"
// 生效中：已签发，护士可执行
#define STATUS_ACTIVE    "ACTIVE"
// 已停嘱：医生停止，不再执行
#define STATUS_STOPPED   "STOPPED"
// 已撤回：医生撤回，作废
#define STATUS_CANCELLED "CANCELLED"

// ==================== 表名常量 ====================
// 药品医嘱表（med_medication_request）
#define TABLE_MEDICATION "med_medication_request"
// 服务医嘱表（wkf_service_request）
#define TABLE_SERVICE    "wkf_service_request"

/**
 * 签发医嘱
 * 将待签发（DRAFT）的医嘱改为生效中（ACTIVE）
 * 支持批量签发，传入signTargets数组，每条包含表名和ID
 * 任一失败则整体失败，返回false
 * 
 * 状态流转：DRAFT(待签发) -> ACTIVE(生效中)
 */
bool MedicalOrderService::sign(const SignAdviceDTO::Wrapper& dto) {
	for (auto& item : *dto->signTargets.get()) {
		std::string tableName = item->productTableName.getValue("");
		std::string id = item->id.getValue("");

		if (tableName == TABLE_MEDICATION)
		{
			MedicationDAO dao;
			auto record = dao.selectById(id);                // 1. 查询记录
			if (!record) return false;                       // 记录不存在
			if (record->getStatusEnum() != STATUS_DRAFT) return false; // 只允许签发DRAFT状态的医嘱
			record->setStatusEnum(STATUS_ACTIVE);            // 2. 改状态为ACTIVE
			if (dao.update(*record) != 1) return false;      // 3. 更新数据库
		}
		else if (tableName == TABLE_SERVICE)
		{
			ServiceRequestDAO dao;
			auto record = dao.selectById(id);
			if (!record) return false;
			if (record->getStatusEnum() != STATUS_DRAFT) return false;
			record->setStatusEnum(STATUS_ACTIVE);
			if (dao.update(*record) != 1) return false;
		}
	}
	return true;
}

/**
 * 撤回医嘱
 * 将生效中（ACTIVE）的医嘱改为已撤回（CANCELLED）
 * 支持批量撤回，传入targets数组，每条包含表名和ID
 * 任一失败则整体失败，返回false
 * 
 * 状态流转：ACTIVE(生效中) -> CANCELLED(已撤回)
 */
bool MedicalOrderService::withdraw(const WithdrawAdviceDTO::Wrapper& dto) {
	for (auto& item : *dto->targets.get())
	{
		std::string tableName = item->productTableName.getValue("");
		std::string id = item->id.getValue("");

		if (tableName == TABLE_MEDICATION)
		{
			MedicationDAO dao;
			auto record = dao.selectById(id);                // 1. 查询记录
			if (!record) return false;                       // 记录不存在
			if (record->getStatusEnum() != STATUS_ACTIVE) return false; // 只允许撤回ACTIVE状态的医嘱
			record->setStatusEnum(STATUS_CANCELLED);         // 2. 改状态为CANCELLED
			if (dao.update(*record) != 1) return false;      // 3. 更新数据库
		}
		else if (tableName == TABLE_SERVICE)
		{
			ServiceRequestDAO dao;
			auto record = dao.selectById(id);
			if (!record) return false;
			if (record->getStatusEnum() != STATUS_ACTIVE) return false;
			record->setStatusEnum(STATUS_CANCELLED);
			if (dao.update(*record) != 1) return false;
		}
	}
	return true;
}

/**
 * 取消停嘱
 * 将已停嘱（STOPPED）的医嘱恢复为生效中（ACTIVE）
 * 支持批量操作，传入lines数组，每条包含表名和ID
 * 任一失败则整体失败，返回false
 * 
 * 状态流转：STOPPED(已停嘱) -> ACTIVE(生效中)
 */
bool MedicalOrderService::cancelStop(const CancelStopAdviceDTO::Wrapper& dto) {
	for (auto& item : *dto->lines.get())
	{
		std::string tableName = item->productTableName.getValue("");
		std::string id = item->id.getValue("");

		if (tableName == TABLE_MEDICATION)
		{
			MedicationDAO dao;
			auto record = dao.selectById(id);                // 1. 查询记录
			if (!record) return false;                       // 记录不存在
			if (record->getStatusEnum() != STATUS_STOPPED) return false; // 只允许恢复STOPPED状态的医嘱
			record->setStatusEnum(STATUS_ACTIVE);            // 2. 改状态为ACTIVE
			if (dao.update(*record) != 1) return false;      // 3. 更新数据库
		}
		else if (tableName == TABLE_SERVICE)
		{
			ServiceRequestDAO dao;
			auto record = dao.selectById(id);
			if (!record) return false;
			if (record->getStatusEnum() != STATUS_STOPPED) return false;
			record->setStatusEnum(STATUS_ACTIVE);
			if (dao.update(*record) != 1) return false;
		}
	}
	return true;
}