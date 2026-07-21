#include "stdafx.h"
#include "id/SnowFlake.h"
#include "OrderExecuteControlService.h"

#include <unordered_set>

#include "dao/medicalorderexecution/OrderExecuteControlDAO.h"
#include "SimpleDateTimeFormat.h"

//TODO - 没加当前操作者权限范围校验，为了避免"越权登记别的病区医嘱为“不执行”"
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

	OrderExecuteControlDAO dao;

	//开启事务，只允许批量成功/失败
	dao.getSqlSession()->beginTransaction();
	try
	{
		for (auto& dto : *dtolist)
		{
			string requestTable;
			if (*dto->advice_type.getPtr() == "MED_REQUEST")
				requestTable = "med_medication_request";
			else if (*dto->advice_type.getPtr() == "SERVICE_REQUEST")
				requestTable = "wkf_service_request";
			else
			{
				dao.getSqlSession()->rollbackTransaction();
				return "ORDER_NOT_EXECUTE_ADVICE_TYPE_INVALID";
			}

			/*调用DAO层查询原医嘱，验证医嘱存在以及获取后续业务所需关键字段*/
			auto originalOrder = dao.queryOriginalOrderRequestById(*dto->advice_id.getPtr(), *dto->advice_type.getPtr());
			if (!originalOrder) { //这条不执行请求没有查询对应医嘱
				dao.getSqlSession()->rollbackTransaction();
				return "ORDER_NOT_EXECUTE_ORIGINAL_ORDER_NOT_FOUND";
			}

			/*pending净态校验，查询执行记录表是否存在同id已执行或者不执行*/
			auto occupiedProcedure = dao.queryActiveTerminalProcedureBySlot(
				*dto->advice_id.getPtr(),
				requestTable,
				*dto->plan_time.getPtr());
			if (occupiedProcedure)
			{
				dao.getSqlSession()->rollbackTransaction();
				if (occupiedProcedure->getStatusEnum() == "COMPLETED")
					return "ORDER_NOT_EXECUTE_SLOT_ALREADY_COMPLETED";
				if (occupiedProcedure->getStatusEnum() == "STOPPED")
					return "ORDER_NOT_EXECUTE_SLOT_ALREADY_STOPPED";
				return "ORDER_NOT_EXECUTE_SLOT_STATE_CHANGED";
			}

			/*生成雪花id*/
			string procedureId = std::to_string(SnowFlake::getInstance()->nextId());
			if (procedureId == "0")
			{
				dao.getSqlSession()->rollbackTransaction();
				return "ORDER_NOT_EXECUTE_SYSTEM_ERROR";
			}

			string nowTime = SimpleDateTimeFormat::formatWithMilli("%Y-%m-%d %H:%M:%S", "."); //获取当前毫秒级时间
			
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
				*dto->plan_time.getPtr());

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

std::string OrderExecuteControlService::buildNotExecuteRequestKey(const OrderNotExecuteDTO::Wrapper& dto) const
{
	return *dto->advice_id.getPtr() + "|" + *dto->advice_type.getPtr() + "|" + *dto->plan_time.getPtr();
}
