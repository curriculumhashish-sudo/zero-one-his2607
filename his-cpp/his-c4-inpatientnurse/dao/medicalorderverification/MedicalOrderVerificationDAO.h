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
#pragma once
#ifndef _MEDICAL_ORDER_VERIFICATION_DAO_H_
#define _MEDICAL_ORDER_VERIFICATION_DAO_H_
#include "BaseDAO.h"
#include "domain/query/medicalorderverification/MedicalOrderVerificationQuery.h"
#include "domain/dto/medicalorderverification/MedicalOrderVerificationDetailDTO.h"
#include "domain/dto/medicalorderverification/VerificationDTO.h"

class MedicalOrderVerificationDAO : public BaseDAO
{
public:
	std::list<MedicalOrderVerificationDetailDTO::Wrapper> selectItems(const MedicalVerificationQuery::Wrapper& query);
	std::list<MedicalOrderVerificationDetailDTO::Wrapper> selectWorkbenchItems(
		const MedicalVerificationWorkbenchQueryDTO::Wrapper& query);
	std::string selectEncounterId(const std::string& orderType,
		const std::string& requestId);
	std::list<VerificationWriteItemDTO::Wrapper> selectActiveTcmPrescriptionLines(
		const std::string& requestId, const std::string& encounterId);
	bool hasWriteState(const std::string& orderType, const std::string& requestId,
		const std::string& encounterId, const std::string& statusEnum,
		const std::string& operatorId);
	int verify(const std::string& orderType,const std::string& requestId,
		const std::string& encounterId,const oatpp::Int64& expectedVersion,
		const std::string& operatorId);
	int reject(const std::string& orderType,const std::string& requestId,
		const std::string& encounterId,const oatpp::Int64& expectedVersion,
		const std::string& reason,const std::string& operatorId);
	std::list<std::string> selectIdempotencyResult(
		const std::string& operation, const std::string& idempotencyKey);
	int insertIdempotencyResult(
		const std::string& auditId, const std::string& operation,
		const std::string& idempotencyKey, const std::string& operatorId,
		const std::string& fingerprint, const std::string& resultData);
};

#endif // !_MEDICAL_ORDER_VERIFICATION_DAO_H_
