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
ExecuteInpatientDischargeResponseDTO::Wrapper AdmissionTransferService::discharge(const ExecuteInpatientDischargeDTO::Wrapper& dto,const std::string& op)
{
	auto out=ExecuteInpatientDischargeResponseDTO::createShared();out->encounterId=dto?dto->encounterId:"";out->succeeded=false;if(!dto||!dto->encounterId||dto->encounterId->empty())return out;
	AdmissionTransferDAO dao;auto status=dao.selectEncounterStatus(dto->encounterId);if(status=="PENDING"){out->succeeded=true;return out;}if(status!="ON_HOLD")return out;
	if(dao.countUnstoppedLongOrders(dto->encounterId)>0||dao.countUnfinishedTemporaryOrders(dto->encounterId)>0||dao.countPendingMedicationDispenses(dto->encounterId)>0)return out;
	out->succeeded=dao.markPendingDischarge(dto->encounterId,op)==1;return out;
}
ClearDischargedBedResponseDTO::Wrapper AdmissionTransferService::clearBed(const ClearDischargedBedDTO::Wrapper& dto,const std::string& op)
{
	auto out=ClearDischargedBedResponseDTO::createShared();out->encounterId=dto?dto->encounterId:"";out->succeeded=false;if(!dto||!dto->encounterId||dto->encounterId->empty())return out;
	AdmissionTransferDAO dao;if(dao.selectEncounterStatus(dto->encounterId)!="COMPLETED"||dao.selectDisplayBalance(dto->encounterId)>0.000001)return out;auto bed=dao.selectActiveBedId(dto->encounterId);if(bed.empty())return out;
	dao.getSqlSession()->beginTransaction();try{if(dao.closeBedTrack(dto->encounterId,bed,op)!=1||dao.releaseBed(bed,op)!=1||dao.markDischarged(dto->encounterId,op)!=1)throw std::runtime_error("clear bed failed");dao.getSqlSession()->commitTransaction();out->succeeded=true;}catch(...){dao.getSqlSession()->rollbackTransaction();}return out;
}
