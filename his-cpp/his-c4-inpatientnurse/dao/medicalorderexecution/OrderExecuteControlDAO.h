#pragma once

#ifndef _ORDER_EXECUTE_CONTROL_DAO_
#define _ORDER_EXECUTE_CONTROL_DAO_

#include "BaseDAO.h"
#include "domain/do/medicalorderexecution/ClnProcedureDO.h"
#include "domain/do/medicalorderexecution/OriginalOrderRequestDO.h"

class OrderExecuteControlDAO : public BaseDAO
{
public:
	// 查询医嘱执行管控业务需要的原医嘱关键数据
	PtrOriginalOrderRequestDO queryOriginalOrderRequestById(const string& orderId, const string& adviceType);

	// 查询同一计划点是否已存在占用中的有效终态执行记录
	PtrClnProcedureDO queryActiveTerminalProcedureBySlot(
		const string& requestId,
		const string& requestTable,
		const string& occurrenceTime);

	// 根据执行记录ID查询有效的已执行记录
	PtrClnProcedureDO queryCompletedProcedureById(const string& procedureId);

	// 判断被对冲的执行记录是否已经存在有效取消执行记录
	bool existsCancelProcedureByRefundId(const string& refundId);
};

#endif // !_ORDER_EXECUTE_CONTROL_DAO_
