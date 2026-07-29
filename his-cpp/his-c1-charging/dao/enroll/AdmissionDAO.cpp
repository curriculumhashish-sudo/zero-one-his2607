#include "stdafx.h"
#include "AdmissionDAO.h"
#include "id/UuidFacade.h"
#include <sstream>
#include <iomanip>
#include <ctime>

std::string AdmissionDAO::generateUuid()
{
	UuidFacade uuidFacade;
	return uuidFacade.genUuid(true);
}

std::string AdmissionDAO::generateBusNo(const oatpp::String& prefix)
{
	std::time_t now = std::time(nullptr);
	std::tm tm;
#ifdef LINUX
	localtime_r(&now, &tm);
#else
	localtime_s(&tm, &now);
#endif
	std::ostringstream oss;
	oss << (prefix && !prefix->empty() ? prefix->c_str() : "ZY")
		<< std::setfill('0') << std::setw(4) << (tm.tm_year + 1900)
		<< std::setw(2) << (tm.tm_mon + 1)
		<< std::setw(2) << tm.tm_mday
		<< std::setw(4) << (std::rand() % 10000);
	return oss.str();
}

// 添加字符串参数
static void sp(SqlParams& p, const oatpp::String& v) {
	p.push_back(SqlParam("s", std::make_shared<std::string>(v ? v->c_str() : "")));
}

bool AdmissionDAO::insertAdmission(
	const oatpp::String& name,
	const oatpp::String& idCard,
	const oatpp::String& birthTime,
	const oatpp::Int32& age,
	const oatpp::String& sex,
	const oatpp::String& phone,
	const oatpp::String& admissionDept,
	const oatpp::String& admissionWard,
	const oatpp::String& attendingDoctor,
	const oatpp::String& admissionDiagnosis,
	const oatpp::String& diagnosisDesc,
	const oatpp::String& patientCondition,
	const oatpp::String& admissionType,
	const oatpp::String& admissionMode,
	const oatpp::String& feeNature,
	const oatpp::String& admissionDate,
	const oatpp::String& prepayAmount,
	const oatpp::String& payWay,
	const oatpp::String& tenantId,
	const oatpp::String& orgId,
	const oatpp::String& operUserId,
	oatpp::String& outPatientId,
	oatpp::String& outEncounterId,
	oatpp::String& outBusNo
)
{
	// 校验必填字段
	if (!name || name->empty()) return false;
	if (!sex || sex->empty()) return false;
	if (age && age.getValue(0) < 0) return false;

	// 生成主键和业务编号
	std::string patientId = generateUuid();
	std::string encounterId = generateUuid();
	std::string encounterBusNo = generateBusNo(oatpp::String("ZY"));
	std::string accountBusNo = generateBusNo(oatpp::String("AC"));

	// 设置输出参数
	outPatientId = oatpp::String(patientId);
	outEncounterId = oatpp::String(encounterId);
	outBusNo = oatpp::String(encounterBusNo);

	// 开启事务
	sqlSession->beginTransaction();

	try
	{
		std::string sql;
		SqlParams params;

		// 插入患者表
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
		sp(params, std::string("BR") + encounterBusNo.substr(2)); // 患者业务编号：BR + 日期 + 随机数
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

		// 插入患者标识表
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

		//  插入就诊表
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

		// 插入就诊诊断表
		sql = R"(
INSERT INTO adm_encounter_diagnosis
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, condition_id, primary_flag, rank, description,
 diag_type_code, med_type, highest_diag_flag, tcm_flag)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, NULL, 1, 1, ?,
 'ADMIT_DIAG', 'WEST', 1, 0)
)";
		params.clear();
		sp(params, generateUuid());
		sp(params, tenantId);
		sp(params, operUserId);
		sp(params, operUserId);
		sp(params, encounterId);
		sp(params, diagnosisDesc);

		if (sqlSession->executeUpdate(sql, params) < 1) {
			sqlSession->rollbackTransaction();
			return false;
		}

		//  插入就诊参与人表
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

		// 插入就诊位置表
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

		// 插入账户表
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

		// 提交事务 — 所有7个插入操作成功
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
	const oatpp::String& encounterId,
	const oatpp::String& cancelReason,
	const oatpp::String& operUserId,
	const oatpp::String& tenantId
)
{
	// 校验必填字段
	if (!encounterId || encounterId->empty()) return false;
	if (!tenantId || tenantId->empty()) return false;

	sqlSession->beginTransaction();

	try
	{
		std::string sql;
		SqlParams params;

		// 软删除：将所有关联表的 is_deleted 设置为 1
		// 顺序遵循外键依赖（先子表后父表）

		// 1. 账户表
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

		// 2. 就诊位置表
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

		// 3. 就诊参与人表
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

		// 4. 就诊诊断表
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

		// 5. 就诊表（核心就诊记录）
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
