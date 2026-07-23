#pragma once

#ifndef _ORDER_EXECUTE_CONTROL_DAO_
#define _ORDER_EXECUTE_CONTROL_DAO_

#include <list>
#include <string>
#include <tuple>
#include <utility>

#include "BaseDAO.h"
#include "domain/do/medicalorderexecution/ClnProcedureDO.h"
#include "domain/do/medicalorderexecution/OriginalOrderRequestDO.h"

class OrderExecuteControlDAO : public BaseDAO
{
public:
	// 批量查询医嘱执行管控业务需要的原医嘱关键数据
	std::list<std::pair<std::string, PtrOriginalOrderRequestDO>> queryOriginalOrderRequestById(
		const std::list<std::string>& orderIds,
		const string& adviceType);

	// 批量判断计划点是否已被有效终态执行记录占用
	bool existsActiveTerminalProcedureBySlot(
		const std::list<std::tuple<std::string, std::string, std::string>>& slots);

	// 根据执行记录ID批量查询有效的已执行记录
	std::list<PtrClnProcedureDO> queryCompletedProcedureById(const std::list<std::string>& procedureIds);

	// 检查是否存在已经指向待取消的已执行记录的cancel行
	bool existsCancelProcedureByRefundIds(const std::list<std::string>& refundIds);

	// Insert CANCEL procedure rows without starting or committing a transaction.
	int insertCancelProceduresNoTransaction(const std::list<ClnProcedureDO>& procedures);

	// 根据id将所有已执行记录的occurrence_slot_active计划点占用槽全部置空
	int releaseCompletedProcedureSlotsNoTransaction(const std::list<std::string>& procedureIds);

	/*根据用户 ID 查询该用户能操作的所有住院记录(encounter_id)，用于权限校验*/
	std::list<std::string> listOperableEncounterIdsByUserId(const std::string& userId);
};

#endif // !_ORDER_EXECUTE_CONTROL_DAO_
