#include "stdafx.h"
#include "PrescriptionDAO.h"
#include "WhaleSequenceDAO.h"
#include "ConvertTimestampToMySQLDatetime6.cpp"
#include <sstream>
#include <iomanip>

std::string PrescriptionDAO::queryHistoryConditionBuilder(
	const PrescriptionHistoryQuery::Wrapper& query,
	SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE m3.is_deleted = 0";
	sqlCondition << " AND m3.rx_type_code = '2'";
	sqlCondition << " AND m3.status_enum = 'STOPPED'";
	sqlCondition << " AND m3.prescription_no IS NOT NULL";
	
	sqlCondition << " AND m3.patient_id = ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	sqlCondition << " AND m3.encounter_id <> ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->id.getValue(""));
	
	return sqlCondition.str();
}

std::string PrescriptionDAO::queryCurrentConditionBuilder(
	const PrescriptionCurrentQuery::Wrapper& query,
	SqlParams& params)
{
	std::stringstream sqlCondition;
	sqlCondition << " WHERE is_deleted = 0";
	sqlCondition << " AND rx_type_code = '2'";
	sqlCondition << " AND prescription_no IS NOT NULL";

	sqlCondition << " AND encounter_id = ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->id.getValue(""));
	sqlCondition << " AND patient_id = ?";
	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));

	return sqlCondition.str();
}

uint64_t PrescriptionDAO::countHistory(
	const PrescriptionHistoryQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT COUNT(DISTINCT m3.prescription_no) "
		"FROM med_medication_request m3 "
		"JOIN med_medication_definition m2 ON m2.id = m3.medication_id";
	sql += queryHistoryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

uint64_t PrescriptionDAO::countCurrent(const PrescriptionCurrentQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT COUNT(DISTINCT m1.prescription_no) "
		"FROM med_medication_request m1 ";
	sql += queryCurrentConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<PrescriptionHistorySummaryDO>
PrescriptionDAO::selectHistoryPrescription(
	const PrescriptionHistoryQuery::Wrapper& query)
{
	SqlParams params;
	std::string sql =
		"SELECT "
		"  m3.prescription_no        AS prescription_no, "
		"  m3.group_id			     AS prescriptionGroupKey,"
		"  m3.encounter_id           AS encounter_id, "
		"  MIN(m3.req_authored_time) AS authored_time, "
		"  COUNT(*)                  AS medication_count, "
		"  JSON_ARRAYAGG(JSON_OBJECT('medicine_name', m2.name)) "
		"                            AS medication_names_json "
		"FROM med_medication_request m3 "
		"JOIN med_medication_definition m2 ON m2.id = m3.medication_id";

	sql += queryHistoryConditionBuilder(query, params);

	sql +=
		" GROUP BY m3.prescription_no, m3.encounter_id, m3.group_id"
		" ORDER BY authored_time DESC";

	return sqlSession->executeQuery<PrescriptionHistorySummaryDO>(
		sql, PrescriptionHistorySummaryMapper(), params);
}

std::list<PrescriptionCurrentSummaryDO>
PrescriptionDAO::selectCurrentPrescription(
	const PrescriptionCurrentQuery::Wrapper& query)
{
	SqlParams params;

	std::string sql =
		"SELECT "
		"  m1.prescription_no            AS prescription_no, "
		"  m1.group_id					 AS prescriptionGroupKey,"		
		"  m1.status_enum                AS status_enum, "
		"  m1.therapy_enum               AS therapy_enum, "
		"  t1.display                    AS method_display, "
		"  c1.name                       AS frequency_name, "
		"  m1.dispense_per_duration      AS dispense_per_duration, "
		"  m1.chinese_herbs_dose_quantity AS chinese_herbs_dose_quantity, "
		"  m1.suffering_flag             AS suffering_flag, "
		"  a1.description                AS diagnosis_description, "
		"  m1.remark                     AS remark, "
		"  JSON_ARRAYAGG( "
		"    JSON_ARRAY( "
		"      m1.id, "
		"      m2.id, "
		"      m2.name, "
		"      m1.dose, "
		"      a2.price_value, "
		"      t2.display "
		"    ) "
		"  ) AS medication_details_json "
		"FROM med_medication_request m1 "
		"LEFT JOIN ter_code_system t3 "
		"  ON t3.name = 'tcm_method_code' "
		"  AND t3.tenant_id = m1.tenant_id "
		"  AND t3.is_deleted = false "
		"LEFT JOIN ter_concept t1 "
		"  ON m1.method_code = t1.code AND t1.code_system_id = t3.id "
		"LEFT JOIN ter_code_system t4 "
		"  ON t4.name = 'unit_code' "
		"  AND t4.tenant_id = m1.tenant_id "
		"  AND t4.is_deleted = false "
		"LEFT JOIN ter_concept t2 "
		"  ON m1.unit_code = t2.code AND t2.code_system_id = t4.id "
		"LEFT JOIN cus_frequency c1 ON m1.rate_code = c1.rate_code "
		"LEFT JOIN LATERAL ( "
		"  SELECT description "
		"  FROM adm_encounter_diagnosis a1 "
		"  WHERE a1.encounter_id = m1.encounter_id "
		"  AND a1.tcm_flag = true "
		"  LIMIT 1 "
		") a1 ON TRUE "
		"LEFT JOIN med_medication_definition m2 ON m1.medication_id = m2.id "
		"LEFT JOIN adm_charge_item_definition a2 "
		"  ON a2.instance_table = 'med_medication_definition' "
		"  AND a2.instance_id = m1.medication_id "
		"  AND a2.price_type_code = 'SALE' "
		"  AND a2.is_deleted = false ";

	sql +=
		"WHERE m1.rx_type_code = '2' "
		"  AND m1.patient_id = ? "
		"  AND m1.encounter_id = ? "
		"GROUP BY "
		"  m1.prescription_no, "
		"  m1.status_enum, "
		"  m1.therapy_enum, "
		"  t1.display, "
		"  c1.name, "
		"  m1.dispense_per_duration, "
		"  m1.chinese_herbs_dose_quantity, "
		"  m1.suffering_flag, "
		"  a1.description, "
		"  m1.remark,"
		"  m1.group_id "
		"ORDER BY m1.prescription_no";

	SQLPARAMS_PUSH(params, "s", std::string, query->patientId.getValue(""));
	SQLPARAMS_PUSH(params, "s", std::string, query->id.getValue(""));

	return sqlSession->executeQuery<PrescriptionCurrentSummaryDO>(
		sql, PrescriptionCurrentSummaryMapper(), params);
}

oatpp::List<oatpp::String> PrescriptionDAO::insertNewPrescription(const PrescriptionNewSummaryDO& row)
{
	// Top-level fields shared by every inserted row.
	const std::string encounterId = row.getEncounterId();
	const std::string patientId   = row.getPatientId();
	const std::string createAt    = row.getCreateAt();
	const std::string tenantId    = row.getTenantId();
	const std::string formattedCreateAt = ConvertTimestampToMySQLDatetime6(createAt);

	const auto& prescriptions = row.getPrescriptions();

	// Generate a single prescription_no for this prescription.
	// Format: CP + YYYY-MM-DD + 4-digit zero-padded daily sequence,
	// e.g. CP2026-07-270001, CP2026-07-270002, ...
	// The seqKey embeds the tenant id and date so each day's counter starts at 1
	// per tenant, and the date matches the creation timestamp.
	const std::string dateBuf = formattedCreateAt.substr(0, 10);
	if (dateBuf.size() != 10)
	{
		// Defensive guard: malformed/empty timestamp -> bail out.
		return {};
	}

	const std::string seqKey = std::string("1:PR:") + dateBuf;
	WhaleSequenceDAO seqDao;
	const uint64_t seqVal = seqDao.nextValue(seqKey);

	std::ostringstream noStream;
	noStream << "CP" << dateBuf
		<< std::setw(4) << std::setfill('0') << seqVal;
	const std::string prescriptionNo = noStream.str();

	// 3. 开事务
	SqlSession* session = this->getSqlSession();
	session->beginTransaction();

	try {
		auto idList = oatpp::List<oatpp::String>::createShared();
		for (const auto& pr : prescriptions)
		{
			//if (!pr || !pr->lines) continue;

			MedMedicationRequestDO r;

			// Top-level shared fields.
			r.setEncounterId(encounterId);
			r.setPatientId(patientId);
			r.setTenantId(tenantId);
			r.setPrescriptionNo(prescriptionNo);
			r.setRxTypeCode("2");        // TCM prescription
			r.setStatusEnum("ACTIVE");
			r.setBusNo("");               // Required NOT NULL column

			// Per-prescription fields.
			r.setGroupId(pr->prescriptionGroupKey.getValue(""));
			r.setTherapyEnum(pr->therapyEnum.getValue(""));
			r.setMethodCode(pr->methodCode.getValue(""));
			r.setRateCode(pr->rateCode.getValue(""));
			r.setDispensePerDuration(pr->days.getValue(0));
			r.setChineseHerbsDoseQuantity(pr->chineseHerbsDoseQuantity.getValue(0));
			r.setSufferingFlag(pr->sufferingFlag.getValue(false));
			r.setRemark(pr->remark.getValue(""));

			// Audit fields.
			r.setCreatedAt(formattedCreateAt);
			r.setIsDeleted(0);

			for (const auto& one : *pr->lines)
			{
				r.setMedicationId(one->medicationDefinitionId.getValue(""));
				r.setQuantity(10);
				r.setUnitCode(one->doseUnitCode.getValue(""));
				r.setDose(one->dose.getValue(0.0));

				// generate primary key
				SnowFlake* sf = SnowFlake::getInstance();
				uint64_t id = sf->nextId();
				if (0 == id) {
					throw std::runtime_error("SnowFlake returned 0,abort insert batch.");
				}
				else {
					r.setId(std::to_string(id));
				}

				const int affected = insert(r);
				if (affected > 0)
				{
					idList->emplace_back(oatpp::String(r.getId()));
				}
				else {
					throw std::runtime_error("Insert affected=0,abort insert batch.");
				}
			}
		}

		session->commitTransaction();
		return idList;
	}
	catch (const std::exception& e) {
		std::cerr << "[insertNewPrescription] rollback: " <<
			e.what() << std::endl;
		session->rollbackTransaction();
		return {};
	}
}

int PrescriptionDAO::deleteById(const PrescriptionDeleteSummaryDO& row) {
	const std::string encounterId = row.getEncounterId();
	const std::string patientId   = row.getPatientId();
	const std::string tenantId    = row.getTenantId();

	const auto& ids = row.getMedicationRequestIds();

	SqlParams params;
	std::ostringstream sql;
	sql << "UPDATE med_medication_request "
		<< "SET is_deleted = 1, "
		<< "    deleted_at = NOW(), "
		<< "    deleted_by = ? "
		<< "WHERE is_deleted = 0 "
		<< "  AND encounter_id = ? "
		<< "  AND patient_id = ? "
		<< "  AND tenant_id = ? "
		<< "  AND id IN (";

	// Parameter order follows the SQL text order (deleted_by, then encounter/patient/tenant, then id IN).
	SQLPARAMS_PUSH(params, "s", std::string, std::string(""));
	SQLPARAMS_PUSH(params, "s", std::string, encounterId);
	SQLPARAMS_PUSH(params, "s", std::string, patientId);
	SQLPARAMS_PUSH(params, "s", std::string, tenantId);

	bool isFirst = true;
	for (const auto& one : ids)
	{
		if (one.empty())
		{
			continue;
		}
		if (!isFirst)
		{
			sql << ", ";
		}
		sql << "?";
		SQLPARAMS_PUSH(params, "s", std::string, one);
		isFirst = false;
	}

	if (isFirst)
	{
		// Every id was empty -> nothing to update.
		return 0;
	}

	sql << ")";

	return sqlSession->executeUpdate(sql.str(), params);
}
