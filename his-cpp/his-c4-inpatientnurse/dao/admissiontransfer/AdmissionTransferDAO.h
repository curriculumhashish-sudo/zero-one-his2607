#pragma once
#ifndef _ADMISSION_TRANSFER_DAO_H_
#define _ADMISSION_TRANSFER_DAO_H_
#include "BaseDAO.h"
class AdmissionTransferDAO : public BaseDAO
{
public:
	std::string selectEncounterStatus(const std::string& encounterId);
	uint64_t countUnstoppedLongOrders(const std::string& encounterId);
	uint64_t countUnfinishedTemporaryOrders(const std::string& encounterId);
	uint64_t countPendingMedicationDispenses(const std::string& encounterId);
	double selectDisplayBalance(const std::string& encounterId);
	std::string selectActiveBedId(const std::string& encounterId);
	int markPendingDischarge(const std::string& encounterId,const std::string& operatorId);
	int closeBedTrack(const std::string& encounterId,const std::string& bedId,const std::string& operatorId);
	int releaseBed(const std::string& bedId,const std::string& operatorId);
	int markDischarged(const std::string& encounterId,const std::string& operatorId);
};
#endif
