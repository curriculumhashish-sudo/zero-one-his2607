#pragma once
#ifndef _MEDICATION_COLLECTION_DAO_H_
#define _MEDICATION_COLLECTION_DAO_H_
#include "BaseDAO.h"
#include "domain/query/medicationcollection/PagePendingDispenseSummaryLinesQuery.h"
struct PendingDispenseRow{std::string dispenseId,procedureId,plannedTime,unitCode,medReqId,medicationId,medicationName,specification,groupId,therapy,dosage,method,frequency,locationId,locationName,encounterId,patientId,patientName,bedName;double quantity=0,price=0;};
struct SummarizeDispenseRow{std::string id,encounterId,patientId,medicationId,locationId,unitCode;double quantity=0,price=0;};
class MedicationCollectionDAO : public BaseDAO
{
public:
	uint64_t countPending(const PagePendingDispenseSummaryLinesQuery::Wrapper& query);
	std::list<PendingDispenseRow> selectPending(const PagePendingDispenseSummaryLinesQuery::Wrapper& query);
	std::list<SummarizeDispenseRow> lockSummarizable(const std::vector<std::string>& ids);
	int attachSummary(const std::string& dispenseId,const std::string& summaryNo,const std::string& operatorId);
	int insertSupplyLine(const std::string& id,const std::string& summaryNo,const SummarizeDispenseRow& row,const std::string& operatorId);
	uint64_t countSummarySupplyLines(const std::string& summaryNo);
	int submitSummarySupplyLines(const std::string& summaryNo,const std::string& operatorId);
	uint64_t countSummaryDispenses(const std::string& summaryNo);
	int releaseSummaryDispenses(const std::string& summaryNo,const std::string& operatorId);
	int removePendingSummarySupplyLines(const std::string& summaryNo,const std::string& operatorId);
};
#endif
