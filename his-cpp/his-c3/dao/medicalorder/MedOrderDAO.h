#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: PKQ
 @Date: 2026/07/22 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _MEDORDERDAO_H_
#define _MEDORDERDAO_H_

#include "BaseDAO.h"
#include "domain/do/medicalorder/MedMedicationRequestDO.h"
#include "domain/query/medicalorder/MedicalOrderQuery.h"

//用于med_medication_request表的数据访问
class MedOrderDAO : public BaseDAO
{
public:
	//按查询统计订单数
	uint64_t count(const MedicalOrderQuery::Wrapper& query);
	//按查询分页列出订单数
	std::list<PtrMedMedicationRequestDO> selectAll(const MedicalOrderQuery::Wrapper& query);
	//按ID选择单个订单
	PtrMedMedicationRequestDO selectById(const std::string& id);
	//选择同一组的订单
	std::list<PtrMedMedicationRequestDO> selectByGroupId(const std::string& groupId);
	//批量更新组ID（合并）
	int updateGroupIdBatch(const std::list<std::string>& orderIds, const std::string& groupId);
	//批量清除组ID（拆分）
	int clearGroupIdBatch(const std::list<std::string>& orderIds);
	//批量更新状态
	int stopOrderBatch(const std::list<std::string>& orderIds, int stopReason, const std::string& operatorId);

};


#endif