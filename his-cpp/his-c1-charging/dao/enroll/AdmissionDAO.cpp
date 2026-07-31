#include "stdafx.h"
#include "AdmissionDAO.h"
#include "SqlSession.h"
#include "id/UuidFacade.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

std::string AdmissionDAO::generateUuid()
{
	UuidFacade uuidFacade;
	return uuidFacade.genUuid(true);
}

std::string AdmissionDAO::generateBusNo(const std::string& prefix)
{
	// Seed rand() once per process
	static bool randSeeded = (std::srand(static_cast<unsigned>(std::time(nullptr))), true);
	(void)randSeeded; // Suppress unused variable warning

	std::time_t now = std::time(nullptr);
	std::tm tm;
#ifdef LINUX
	localtime_r(&now, &tm);
#else
	localtime_s(&tm, &now);
#endif
	std::ostringstream oss;
	oss << (!prefix.empty() ? prefix : "ZY")
		<< std::setfill('0') << std::setw(4) << (tm.tm_year + 1900)
		<< std::setw(2) << (tm.tm_mon + 1)
		<< std::setw(2) << tm.tm_mday
		<< std::setw(4) << (std::rand() % 10000);
	return oss.str();
}

// Append a string-type SqlParam from std::string
void AdmissionDAO::sp(SqlParams& p, const std::string& v) {
	p.push_back(SqlParam("s", std::make_shared<std::string>(v)));
}

// Check if a string matches YYYY-MM-DD date format
static bool isValidDate(const std::string& s)
{
	if (s.length() != 10) return false;
	if (s[4] != '-' || s[7] != '-') return false;
	for (int i = 0; i < 10; ++i) {
		if (i == 4 || i == 7) continue;
		if (s[i] < '0' || s[i] > '9') return false;
	}
	return true;
}

bool AdmissionDAO::insertAdmission(
	const std::string& name,
	const std::string& idCard,
	const std::string& birthTime,
	int age,
	const std::string& sex,
	const std::string& phone,
	const std::string& admissionDept,
	const std::string& admissionWard,
	const std::string& attendingDoctor,
	const std::string& admissionDiagnosis,
	const std::string& diagnosisDesc,
	const std::string& patientCondition,
	const std::string& admissionType,
	const std::string& admissionMode,
	const std::string& feeNature,
	const std::string& admissionDate,
	const std::string& prepayAmount,
	const std::string& payWay,
	const std::string& tenantId,
	const std::string& orgId,
	const std::string& operUserId,
	std::string& outPatientId,
	std::string& outEncounterId,
	std::string& outBusNo
)
{
	// Validate required fields
	if (name.empty()) return false;
	if (sex.empty()) return false;
	if (age < 0) return false;

	// Validate date formats before they reach STR_TO_DATE in SQL
	if (!isValidDate(admissionDate)) {
		std::cerr << "admissionDate is empty or invalid format (expected YYYY-MM-DD): "
		          << admissionDate << std::endl;
		return false;
	}
	if (!isValidDate(birthTime)) {
		std::cerr << "birthTime is empty or invalid format (expected YYYY-MM-DD): "
		          << birthTime << std::endl;
		return false;
	}

	// Generate primary keys and business numbers
	std::string patientId = generateUuid();
	std::string encounterId = generateUuid();
	std::string encounterBusNo = generateBusNo("ZY");
	std::string accountBusNo = generateBusNo("AC");

	// Set output parameters
	outPatientId = patientId;
	outEncounterId = encounterId;
	outBusNo = encounterBusNo;
	// Begin transaction
	sqlSession->beginTransaction();

	try
	{
		std::string sql;
		SqlParams params;

		// 1. Insert patient record
		sql = R"(
INSERT INTO adm_patient
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version, bus_no, name,
 gender_enum, birth_date, status_enum, active_flag, temp_flag,
 id_card, phone, address, nationality_code, organization_id)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1, ?, ?,
 ?, STR_TO_DATE(?,'%Y-%m-%d'), 'NORMAL', 1, 0,
 ?, ?, '', 'CN', ?)
)";
		params.clear();
		sp(params, patientId);
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, std::string("BR") + encounterBusNo.substr(2)); // Patient bus no: BR + date + random
		sp(params, name);
		sp(params, sex);
		sp(params, birthTime);
		sp(params, idCard);
		sp(params, phone);
		sp(params, orgId);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		// 2. Insert patient identifier record
		sql = R"(
INSERT INTO adm_patient_identifier
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 type_code, identifier_no, state_enum, start_time, end_time, patient_id)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 'IDCARD', ?, 'VALID', NOW(6), NULL, ?)
)";
		params.clear();
		sp(params, generateUuid());
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, idCard);
		sp(params, patientId);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		// 3. Insert encounter record
		sql = R"(
INSERT INTO adm_encounter
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 patient_id, group_id, bus_no, status_enum, class_enum,
 admit_source_code, in_way_code, type_enum, start_time, end_time,
 organization_id, priority_enum, reception_time)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, NULL, ?, 'INHOSPITAL', 'COMMON',
 ?, ?, ?, STR_TO_DATE(?,'%Y-%m-%d'), NULL,
 ?, ?, NOW(6))
)";
		params.clear();
		sp(params, encounterId);
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, patientId);
		sp(params, encounterBusNo);
		sp(params, admissionType);
		sp(params, admissionMode);
		sp(params, feeNature);
		sp(params, admissionDate);
		sp(params, orgId);
		sp(params, patientCondition);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		// 4. Insert encounter diagnosis record
		sql = R"(
INSERT INTO adm_encounter_diagnosis
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, condition_id, primary_flag, `rank`, description,
 diag_type_code, med_type, highest_diag_flag, tcm_flag)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
	 ?, ?, 1, 1, ?,
 'ADMIT_DIAG', 'WEST', 1, 0)
)";
		params.clear();
		sp(params, generateUuid());
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, admissionDiagnosis);
		sp(params, diagnosisDesc);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		// 5. Insert encounter participant record
		sql = R"(
INSERT INTO adm_encounter_participant
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, role_code, practitioner_id, status_enum, start_time, end_time)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, 'ATTENDING_DOCTOR', ?, 'ON_DUTY', NOW(6), NULL)
)";
		params.clear();
		sp(params, generateUuid());
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, attendingDoctor);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		// 6. Insert encounter location record
		sql = R"(
INSERT INTO adm_encounter_location
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, location_id, form_enum, status_enum, start_time, end_time)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, ?, 'WARD_BED', 'OCCUPY', NOW(6), NULL)
)";
		params.clear();
		sp(params, generateUuid());
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, admissionWard);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		// 7. Insert account record
		sql = R"(
INSERT INTO adm_account
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 patient_id, encounter_id, type_code, bus_no, name, balance,
 billing_status_enum, encounter_flag, insutype, default_flag, status_enum)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, ?, 'PREPAY', ?, CONCAT(?,'住院预交账户'), ?,
 'UNSETTLE', 1, ?, 1, 'NORMAL')
)";
		params.clear();
		sp(params, generateUuid());
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, patientId);
		sp(params, encounterId);
		sp(params, accountBusNo);
		sp(params, name);
		sp(params, prepayAmount);
		sp(params, feeNature);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		// Commit transaction — all 7 insert operations succeeded
		sqlSession->commitTransaction();
		return true;
	}
	catch (const std::exception& e)
	{
		sqlSession->rollbackTransaction();
		std::cerr << "AdmissionDAO::insertAdmission failed: " << e.what() << std::endl;
		return false;
	}
}

bool AdmissionDAO::deleteAdmission(
	const std::string& encounterId,
	const std::string& cancelReason,
	const std::string& operUserId,
	const std::string& tenantId
)
{
	// Validate required fields
	if (encounterId.empty()) return false;
	if (tenantId.empty()) return false;

	sqlSession->beginTransaction();

	try
	{
		std::string sql;
		SqlParams params;

		// Soft delete: set is_deleted = 1 on all related tables
		// Order follows foreign key dependencies (child tables first, parent last)

		// 1. Account table
		sql = R"(
UPDATE adm_account
SET is_deleted = 1, deleted_at = NOW(6), deleted_by = ?, updated_at = NOW(6)
WHERE encounter_id = ? AND tenant_id = ? AND is_deleted = 0
)";
		params.clear();
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, tenantId);
		sqlSession->executeUpdate(sql, params);

		// 2. Encounter location table
		sql = R"(
UPDATE adm_encounter_location
SET is_deleted = 1, deleted_at = NOW(6), deleted_by = ?, updated_at = NOW(6)
WHERE encounter_id = ? AND tenant_id = ? AND is_deleted = 0
)";
		params.clear();
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, tenantId);
		sqlSession->executeUpdate(sql, params);

		// 3. Encounter participant table
		sql = R"(
UPDATE adm_encounter_participant
SET is_deleted = 1, deleted_at = NOW(6), deleted_by = ?, updated_at = NOW(6)
WHERE encounter_id = ? AND tenant_id = ? AND is_deleted = 0
)";
		params.clear();
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, tenantId);
		sqlSession->executeUpdate(sql, params);

		// 4. Encounter diagnosis table
		sql = R"(
UPDATE adm_encounter_diagnosis
SET is_deleted = 1, deleted_at = NOW(6), deleted_by = ?, updated_at = NOW(6)
WHERE encounter_id = ? AND tenant_id = ? AND is_deleted = 0
)";
		params.clear();
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, tenantId);
		sqlSession->executeUpdate(sql, params);

		// 5. Encounter table (core encounter record)
		sql = R"(
UPDATE adm_encounter
SET is_deleted = 1, deleted_at = NOW(6), deleted_by = ?, updated_at = NOW(6)
WHERE id = ? AND tenant_id = ? AND is_deleted = 0
)";
		params.clear();
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, tenantId);
		sqlSession->executeUpdate(sql, params);

		sqlSession->commitTransaction();
		return true;
	}
	catch (const std::exception& e)
	{
		sqlSession->rollbackTransaction();
		std::cerr << "AdmissionDAO::deleteAdmission failed: " << e.what() << std::endl;
		return false;
	}
}
