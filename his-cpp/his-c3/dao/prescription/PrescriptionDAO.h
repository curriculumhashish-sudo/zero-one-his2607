#pragma once

#ifndef _PRESCRIPTION_DAO_
#define _PRESCRIPTION_DAO_

#include <list>
#include <string>

#include "BaseDAO.h"
#include "../../domain/query/prescription/PrescriptionHistoryQuery.h"
#include "../../domain/query/prescription/PrescriptionCurrentQuery.h"
#include "../../domain/do/prescription/MedMedicationRequestDO (2).h"
#include "../../domain/do/prescription/PrescriptionHistorySummaryDO.h"
#include "../../domain/do/prescription/PrescriptionCurrentSummaryDO.h"
#include "../../domain/do/prescription/PrescriptionNewSummaryDO.h"
#include "../../domain/do/prescription/PrescriptionDeleteSummaryDO.h"
#include "PrescriptionHistorySummaryMapper.h"
#include "PrescriptionCurrentSummaryMapper.h"
#include "id/SnowFlake.h"

/**
 * Queries, inserts, and soft-deletes prescription records.
 */
class PrescriptionDAO : public BaseDAO
{
private:
	std::string queryHistoryConditionBuilder(
		const PrescriptionHistoryQuery::Wrapper& query,
		SqlParams& params);

	std::string queryCurrentConditionBuilder(
		const PrescriptionCurrentQuery::Wrapper& query,
		SqlParams& params);

public:
	// Count historical prescriptions.
	uint64_t countHistory(const PrescriptionHistoryQuery::Wrapper& query);
	// Count current prescriptions.
	uint64_t countCurrent(const PrescriptionCurrentQuery::Wrapper& query);
	// Query historical prescriptions.
	std::list<PrescriptionHistorySummaryDO> selectHistoryPrescription(
		const PrescriptionHistoryQuery::Wrapper& query);
	// Query current prescriptions.
	std::list<PrescriptionCurrentSummaryDO> selectCurrentPrescription(
		const PrescriptionCurrentQuery::Wrapper& query);
	// Insert a new prescription. Returns the generated medication request ids.
	oatpp::List<oatpp::String> insertNewPrescription(
		const PrescriptionNewSummaryDO& row);
	// Soft-delete prescriptions by medication request ids.
	int deleteById(const PrescriptionDeleteSummaryDO& row);
};

#endif // !_PRESCRIPTION_DAO_