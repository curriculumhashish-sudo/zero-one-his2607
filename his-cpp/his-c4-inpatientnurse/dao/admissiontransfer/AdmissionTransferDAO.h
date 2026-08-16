#pragma once
#ifndef _ADMISSION_TRANSFER_DAO_H_
#define _ADMISSION_TRANSFER_DAO_H_
#include "BaseDAO.h"
#include "domain/dto/admissiontransfer/AdmissionTransferWorkbenchDTO.h"
class AdmissionTransferDAO : public BaseDAO
{
public:
	std::string selectEncounterStatus(const std::string& encounterId);
	std::string selectEncounterStatusForUpdate(const std::string& encounterId);
	uint64_t countUnstoppedLongOrders(const std::string& encounterId);
	uint64_t countUnfinishedTemporaryOrders(const std::string& encounterId);
	uint64_t countPendingMedicationDispenses(const std::string& encounterId);
	double selectDisplayBalance(const std::string& encounterId);
	std::string selectActiveBedId(const std::string& encounterId);
	std::string selectActiveBedIdForUpdate(const std::string& encounterId);
	int markPendingDischarge(const std::string& encounterId,const std::string& operatorId);
	int markSettlementCompleted(const std::string& encounterId,const std::string& operatorId);
	int closeBedTrack(const std::string& encounterId,const std::string& bedId,const std::string& operatorId);
	int releaseBed(const std::string& bedId,const std::string& operatorId);
	int markDischarged(const std::string& encounterId,const std::string& operatorId);
	std::list<BedBoardItemDTO::Wrapper> selectBedBoard(
		const std::string& panel, const std::string& wardIds,
		const std::string& houseIds, const std::string& userId);
	std::list<DischargePendingItemDTO::Wrapper> selectPendingExecutions(const std::string& encounterId);
	std::list<DischargePendingItemDTO::Wrapper> selectPendingMedications(const std::string& encounterId);
	int occupyVacantBed(const std::string& bedId,int64_t version,const std::string& operatorId);
	int releaseCurrentBed(const std::string& encounterId,const std::string& operatorId);
	int closeCurrentBedTrack(const std::string& encounterId,const std::string& operatorId);
	int createBedTrack(const std::string& encounterId,const std::string& bedId,const std::string& operatorId);
	int updateEncounterAfterAssignment(const std::string& encounterId,const std::string& operatorId);
	int recallDischarge(const std::string& encounterId,const std::string& operatorId);
	std::string selectBedHouseId(const std::string& bedId);
	std::string selectBedHouseIdForUpdate(const std::string& bedId);
	int lockOccupiedBed(const std::string& bedId,const std::string& operatorId);
	int touchOccupiedBed(const std::string& bedId,int64_t version,const std::string& operatorId);
	int swapBedTracks(const std::string& encounterId,const std::string& sourceBedId,
		const std::string& swapEncounterId,const std::string& targetBedId,const std::string& operatorId);
	int replaceEncounterParticipants(const std::string& encounterId,
		const oatpp::List<oatpp::Object<BedOperationParticipantDTO>>& participants,
		const std::string& operatorId);
	int insertAdmissionVitalSigns(const std::string& encounterId,
		const oatpp::Object<AdmissionVitalSignDTO>& vitalSigns,
		const std::string& operatorId);
	std::string selectBedOperationIdempotency(const std::string& idempotencyKey);
	int insertBedOperationIdempotency(const std::string& idempotencyKey,
		const std::string& operatorId, const std::string& fingerprint,
		const std::string& resultData);
};
#endif
