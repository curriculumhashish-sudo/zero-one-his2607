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
#include "stdafx.h"
#include "AdmissionTransferService.h"
#include "dao/admissiontransfer/AdmissionTransferDAO.h"

namespace {
template<class T>
void setAdmissionError(const T& response, const std::string& code)
{
	response->errorCode = code;
	if (code == "ENCOUNTER_NOT_FOUND")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.encounter-not-found");
	else if (code == "INVALID_DISCHARGE_STATUS")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.invalid-discharge-status");
	else if (code == "LONG_TERM_ORDER_ACTIVE")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.long-term-order-active");
	else if (code == "TEMPORARY_ORDER_PENDING")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.temporary-order-pending");
	else if (code == "MEDICATION_PENDING")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.medication-pending");
	else if (code == "UNSETTLED_CHARGE")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.unsettled-charge");
	else if (code == "ACTIVE_BED_NOT_FOUND")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.active-bed-not-found");
	else if (code == "CONCURRENT_CONFLICT")
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.concurrent-conflict");
	else
		response->errorMessage = ZH_WORDS_GETTER("business-text.admissiontransfer.database-error");
}
}

ExecuteInpatientDischargeResponseDTO::Wrapper AdmissionTransferService::discharge(const ExecuteInpatientDischargeDTO::Wrapper& dto,const std::string& op)
{
	auto out=ExecuteInpatientDischargeResponseDTO::createShared();
	out->encounterId=dto?dto->encounterId:"";
	out->succeeded=false;
	out->bedReleased=false;
	out->previousStatusEnum="";
	out->targetStatusEnum="";
	out->errorCode="";
	out->errorMessage="";
	if(!dto||!dto->encounterId||dto->encounterId->empty())return out;
	AdmissionTransferDAO dao;
	dao.getSqlSession()->beginTransaction();
	try{
		auto status=dao.selectEncounterStatusForUpdate(dto->encounterId);
		out->previousStatusEnum=status;
		out->targetStatusEnum=status;
		if(status=="PENDING"){
			dao.getSqlSession()->commitTransaction();
			out->succeeded=true;
			return out;
		}
		if(status.empty()) throw std::runtime_error("ENCOUNTER_NOT_FOUND");
		if(status!="ON_HOLD") throw std::runtime_error("INVALID_DISCHARGE_STATUS");
		if(dao.countUnstoppedLongOrders(dto->encounterId)>0)
			throw std::runtime_error("LONG_TERM_ORDER_ACTIVE");
		if(dao.countUnfinishedTemporaryOrders(dto->encounterId)>0)
			throw std::runtime_error("TEMPORARY_ORDER_PENDING");
		if(dao.countPendingMedicationDispenses(dto->encounterId)>0)
			throw std::runtime_error("MEDICATION_PENDING");
		if(dao.markPendingDischarge(dto->encounterId,op)!=1)
			throw std::runtime_error("CONCURRENT_CONFLICT");
		dao.getSqlSession()->commitTransaction();
		out->succeeded=true;
		out->targetStatusEnum="PENDING";
	}catch(const std::exception& e){
		dao.getSqlSession()->rollbackTransaction();
		setAdmissionError(out, e.what());
	}catch(...){
		dao.getSqlSession()->rollbackTransaction();
		setAdmissionError(out, "DATABASE_ERROR");
	}
	return out;
}
ClearDischargedBedResponseDTO::Wrapper AdmissionTransferService::clearBed(const ClearDischargedBedDTO::Wrapper& dto,const std::string& op)
{
	auto out=ClearDischargedBedResponseDTO::createShared();
	out->encounterId=dto?dto->encounterId:"";out->succeeded=false;out->bedReleased=false;
	out->previousStatusEnum="";out->targetStatusEnum="";
	out->errorCode="";out->errorMessage="";
	if(!dto||!dto->encounterId||dto->encounterId->empty())return out;
	AdmissionTransferDAO dao;
	dao.getSqlSession()->beginTransaction();
	try{
		auto status=dao.selectEncounterStatusForUpdate(dto->encounterId);
		out->previousStatusEnum=status;out->targetStatusEnum=status;
		auto bed=dao.selectActiveBedIdForUpdate(dto->encounterId);
		if(status.empty()) throw std::runtime_error("ENCOUNTER_NOT_FOUND");
		if(status=="DISCHARGED"){
			dao.getSqlSession()->commitTransaction();
			out->succeeded=true;out->bedReleased=bed.empty();return out;
		}
		if(status=="PENDING"){
			if(dao.selectDisplayBalance(dto->encounterId)>0.000001)
				throw std::runtime_error("UNSETTLED_CHARGE");
			if(dao.markSettlementCompleted(dto->encounterId,op)!=1)
				throw std::runtime_error("CONCURRENT_CONFLICT");
			status="COMPLETED";
			out->targetStatusEnum=status;
		}
		if(status!="COMPLETED") throw std::runtime_error("INVALID_DISCHARGE_STATUS");
		if(dao.selectDisplayBalance(dto->encounterId)>0.000001)
			throw std::runtime_error("UNSETTLED_CHARGE");
		if(bed.empty()) throw std::runtime_error("ACTIVE_BED_NOT_FOUND");
		if(dao.lockOccupiedBed(bed,op)!=1
			||dao.closeBedTrack(dto->encounterId,bed,op)!=1
			||dao.releaseBed(bed,op)!=1
			||dao.markDischarged(dto->encounterId,op)!=1){
			throw std::runtime_error("CONCURRENT_CONFLICT");
		}
		dao.getSqlSession()->commitTransaction();
		out->succeeded=true;out->bedReleased=true;out->targetStatusEnum="DISCHARGED";
	}catch(const std::exception& e){
		dao.getSqlSession()->rollbackTransaction();
		setAdmissionError(out, e.what());
	}catch(...){
		dao.getSqlSession()->rollbackTransaction();
		setAdmissionError(out, "DATABASE_ERROR");
	}
	return out;
}
BedBoardDTO::Wrapper AdmissionTransferService::queryBedBoard(
	const BedBoardQuery::Wrapper& q, const std::string& userId)
{
	auto out=BedBoardDTO::createShared();
	if(!q||!q->panel||(q->panel!="ADMISSION"&&q->panel!="DISCHARGE"))return out;
	out->items=oatpp::List<BedBoardItemDTO::Wrapper>::createShared();
	for(auto& item:AdmissionTransferDAO().selectBedBoard(
		q->panel,q->wardIds.getValue(""),q->houseIds.getValue(""),userId))
		out->items->push_back(item);
	return out;
}
BedOperationResponseDTO::Wrapper AdmissionTransferService::executeBedOperation(
	const ExecuteBedOperationDTO::Wrapper& d,const std::string& op)
{
	auto out=BedOperationResponseDTO::createShared();out->succeeded=false;
	if(!d||!d->encounterId||d->encounterId->empty()||!d->targetBedId||d->targetBedId->empty()
		||!d->targetHouseId||d->targetHouseId->empty()||!d->operationType||d->operationType->empty()
		||!d->idempotencyKey||d->idempotencyKey->empty()||!d->expectedBedVersion)return out;
	out->encounterId=d->encounterId;out->targetBedId=d->targetBedId;
	const std::string encounterId=d->encounterId.getValue("");
	const std::string targetBedId=d->targetBedId.getValue("");
	const std::string operation=d->operationType.getValue("");
	AdmissionTransferDAO dao;
	std::ostringstream fingerprint;
	fingerprint<<operation<<'|'<<encounterId<<'|'<<targetBedId<<'|'
		<<d->targetHouseId.getValue("")<<'|'<<d->swapEncounterId.getValue("")<<'|'
		<<d->expectedBedVersion.getValue(-1)<<'|'<<d->priorityEnum.getValue("");
	for(const auto& participant:*d->participants)
		fingerprint<<"|P:"<<participant->practitionerId.getValue("")<<':'
			<<participant->roleCode.getValue("");
	if(d->admissionVitalSigns)
		fingerprint<<"|V:"<<d->admissionVitalSigns->temperature.getValue("")<<':'
			<<d->admissionVitalSigns->pulse.getValue("")<<':'
			<<d->admissionVitalSigns->heartRate.getValue("")<<':'
			<<d->admissionVitalSigns->bloodPressureHigh.getValue("")<<':'
			<<d->admissionVitalSigns->bloodPressureLow.getValue("")<<':'
			<<d->admissionVitalSigns->heightCm.getValue("")<<':'
			<<d->admissionVitalSigns->weightKg.getValue("");
	const std::string stored=dao.selectBedOperationIdempotency(
		d->idempotencyKey.getValue(""));
	if(!stored.empty()){
		const std::string marker="\n--RESULT--\n";
		const auto position=stored.find(marker);
		if(position==std::string::npos||stored.substr(0,position)!=fingerprint.str()){
			out->errorCode="IDEMPOTENCY_CONFLICT";
			out->errorMessage=ZH_WORDS_GETTER(
				"business-text.admissiontransfer.idempotency-conflict");
			return out;
		}
		out->succeeded=true;
		return out;
	}
	dao.getSqlSession()->beginTransaction();
	try{
		if(dao.selectBedHouseIdForUpdate(targetBedId)!=d->targetHouseId.getValue(""))
			throw std::runtime_error("Target bed does not belong to the selected room.");
		const auto status=dao.selectEncounterStatusForUpdate(encounterId);
		const auto current=dao.selectActiveBedIdForUpdate(encounterId);
		if(current==targetBedId){
			if(dao.insertBedOperationIdempotency(d->idempotencyKey.getValue(""),
				op,fingerprint.str(),encounterId+"|"+targetBedId)!=1)
				throw std::runtime_error("IDEMPOTENCY_WRITE_FAILED");
			dao.getSqlSession()->commitTransaction();out->succeeded=true;return out;
		}
		if(operation=="SWAP"){
			if(current.empty()||!d->swapEncounterId||d->swapEncounterId->empty()
				||(status!="IN_PROGRESS"&&status!="ON_HOLD")||dao.lockOccupiedBed(current,op)!=1)
				throw std::runtime_error("Invalid bed swap.");
			const std::string swapEncounterId=d->swapEncounterId.getValue("");
			const auto swapStatus=dao.selectEncounterStatusForUpdate(swapEncounterId);
			if((swapStatus!="IN_PROGRESS"&&swapStatus!="ON_HOLD")
				||dao.selectActiveBedIdForUpdate(swapEncounterId)!=targetBedId
				||dao.touchOccupiedBed(targetBedId,d->expectedBedVersion.getValue(-1),op)!=1
				||dao.swapBedTracks(encounterId,current,swapEncounterId,targetBedId,op)!=2)
				throw std::runtime_error("Bed swap conflict.");
		}else{
			if(operation=="TRANSFER"){
				if((status!="IN_PROGRESS"&&status!="ON_HOLD")||current.empty()
					||dao.lockOccupiedBed(current,op)!=1||dao.releaseCurrentBed(encounterId,op)!=1
					||dao.closeCurrentBedTrack(encounterId,op)!=1)
					throw std::runtime_error("Bed transfer failed.");
			}else if(operation!="ASSIGN"||status!="REGISTERED"||!current.empty()){
				throw std::runtime_error("Invalid bed operation.");
			}
			if(dao.occupyVacantBed(targetBedId,d->expectedBedVersion.getValue(-1),op)!=1
				||dao.createBedTrack(encounterId,targetBedId,op)!=1
				||dao.updateEncounterAfterAssignment(encounterId,op)!=1)
				throw std::runtime_error("Bed assignment conflict.");
			if(operation=="ASSIGN"){
				if(dao.replaceEncounterParticipants(encounterId,d->participants,op)
					!=static_cast<int>(d->participants->size()))
					throw std::runtime_error("Participant assignment failed.");
				if(d->admissionVitalSigns
					&&dao.insertAdmissionVitalSigns(encounterId,d->admissionVitalSigns,op)<0)
					throw std::runtime_error("Admission vital-sign write failed.");
			}
		}
		if(dao.insertBedOperationIdempotency(d->idempotencyKey.getValue(""),
			op,fingerprint.str(),encounterId+"|"+targetBedId)!=1)
			throw std::runtime_error("IDEMPOTENCY_WRITE_FAILED");
		dao.getSqlSession()->commitTransaction();out->succeeded=true;
	}catch(const std::exception& exception){
		dao.getSqlSession()->rollbackTransaction();
		out->errorCode=exception.what();
		out->errorMessage=ZH_WORDS_GETTER(
			"business-text.admissiontransfer.bed-operation-failed");
	}catch(...){
		dao.getSqlSession()->rollbackTransaction();
		out->errorCode="DATABASE_ERROR";
		out->errorMessage=ZH_WORDS_GETTER(
			"business-text.admissiontransfer.database-error");
	}
	return out;
}
RecallInpatientDischargeResponseDTO::Wrapper AdmissionTransferService::recallDischarge(
	const RecallInpatientDischargeDTO::Wrapper& d,const std::string& op)
{
	auto out=RecallInpatientDischargeResponseDTO::createShared();out->succeeded=false;
	out->requiresBedAssignment=true;out->targetStatusEnum="REGISTERED";
	if(!d||!d->encounterId||d->encounterId->empty())return out;
	out->encounterId=d->encounterId;
	AdmissionTransferDAO dao;dao.getSqlSession()->beginTransaction();
	try{
		auto status=dao.selectEncounterStatusForUpdate(d->encounterId);
		if(status=="REGISTERED"){
			dao.getSqlSession()->commitTransaction();out->succeeded=true;return out;
		}
		auto bed=dao.selectActiveBedIdForUpdate(d->encounterId);
		if(!bed.empty()&&(dao.lockOccupiedBed(bed,op)!=1
			||dao.closeBedTrack(d->encounterId,bed,op)!=1||dao.releaseBed(bed,op)!=1))
			throw std::runtime_error("Recall bed release failed.");
		if(dao.recallDischarge(d->encounterId,op)!=1)
			throw std::runtime_error("Discharge recall failed.");
		dao.getSqlSession()->commitTransaction();out->succeeded=true;
	}catch(...){dao.getSqlSession()->rollbackTransaction();}
	return out;
}
oatpp::List<DischargePendingItemDTO::Wrapper> AdmissionTransferService::queryPendingExecutions(const std::string& id){auto out=oatpp::List<DischargePendingItemDTO::Wrapper>::createShared();for(auto& item:AdmissionTransferDAO().selectPendingExecutions(id))out->push_back(item);return out;}
oatpp::List<DischargePendingItemDTO::Wrapper> AdmissionTransferService::queryPendingMedications(const std::string& id){auto out=oatpp::List<DischargePendingItemDTO::Wrapper>::createShared();for(auto& item:AdmissionTransferDAO().selectPendingMedications(id))out->push_back(item);return out;}
