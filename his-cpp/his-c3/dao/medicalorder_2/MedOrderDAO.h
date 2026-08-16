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
#include "domain/do/medicalorder_2/MedMedicationRequestDO.h"
#include "domain/query/medicalorder_2/MedicalOrderQuery.h"

// Medication order data access layer (med_medication_request)
class MedOrderDAO : public BaseDAO
{
public:
	// Count query
	uint64_t count(const MedicalOrderQuery::Wrapper& query);
	// Paginated query
	std::list<PtrMedMedicationRequestDO> selectAll(const MedicalOrderQuery::Wrapper& query);
	// Query by ID
	PtrMedMedicationRequestDO selectById(const std::string& id);
	// Query by group ID
	std::list<PtrMedMedicationRequestDO> selectByGroupId(const std::string& groupId);
	// Batch update group ID (merge)
	int updateGroupIdBatch(const std::list<std::string>& orderIds, const std::string& groupId, const std::string& encounterId, const std::string& patientId);
	// Batch clear group ID (split)
	int clearGroupIdBatch(const std::list<std::string>& orderIds, const std::string& encounterId, const std::string& patientId);
	// Batch stop orders
	int stopOrderBatch(const std::list<std::string>& orderIds, int stopReason, const std::string& operatorId, const std::string& encounterId, const std::string& patientId);

	// Transaction control
	void beginTransaction() { sqlSession->beginTransaction(); }
	void commitTransaction() { sqlSession->commitTransaction(); }
	void rollbackTransaction() { sqlSession->rollbackTransaction(); }

};


#endif