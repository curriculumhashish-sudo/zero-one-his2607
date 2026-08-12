#include "stdafx.h"
#include "AdmissionDAO.h"
#include "Mapper.h"
#include "SqlSession.h"
#include "id/UuidFacade.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <utility>

// Maps one query row into (encounter id, status_enum) pair for cancel validation.
class EncounterStatusMapper : public Mapper<std::pair<std::string, std::string>>
{
public:
    std::pair<std::string, std::string> mapper(ResultSet* resultSet) const override
    {
        return std::make_pair(
            resultSet->getString("id"),
            resultSet->getString("status_enum")
        );
    }
};

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

std::string AdmissionDAO::insertAdmission(
    const std::string& name,
    const std::string& idCard,
    const std::string& birthTime,
    int age,
    const std::string& sex,
    const std::string& phone,
    const std::string& organizationId,
    const std::string& wardId,
    const std::string& attendingDoctorId,
    const std::string& contractId,
    const std::string& admissionDiagnosis,
    const std::string& diagnosisDesc,
    const std::string& patientCondition,
    const std::string& admissionType,
    const std::string& admissionMode,
    const std::string& feeNature,
    const std::string& admissionDate,
    const std::string& prepayAmount,
    const std::vector<AdmissionPaymentDetail>& paymentDetails,
    const std::string& tenantId,
    const std::string& operUserId,
    std::string& outPatientId,
    std::string& outEncounterId,
    std::string& outBusNo
)
{
    // Validate required fields
    if (name.empty()) return "INSERT_FAILED";
    if (sex.empty()) return "INSERT_FAILED";
    if (age < 0) return "INSERT_FAILED";

    // Validate date formats before they reach STR_TO_DATE in SQL
    if (!isValidDate(admissionDate)) {
        std::cerr << "admissionDate is empty or invalid format (expected YYYY-MM-DD): "
                  << admissionDate << std::endl;
        return "DATE_INVALID";
    }
    if (!isValidDate(birthTime)) {
        std::cerr << "birthTime is empty or invalid format (expected YYYY-MM-DD): "
                  << birthTime << std::endl;
        return "DATE_INVALID";
    }

    // Generate primary keys and business numbers
    std::string patientId = generateUuid();
    std::string encounterId = generateUuid();
    std::string accountId = generateUuid();
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
        sp(params, organizationId);

        if (sqlSession->executeUpdate(sql, params) < 1) {
            sqlSession->rollbackTransaction();
            return "INSERT_FAILED";
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
            return "INSERT_FAILED";
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
 ?, NULL, ?, 'INHOSPITAL', 'IMP',
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
        sp(params, organizationId);
        sp(params, patientCondition);

        if (sqlSession->executeUpdate(sql, params) < 1) {
            sqlSession->rollbackTransaction();
            return "INSERT_FAILED";
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
            return "INSERT_FAILED";
        }

        // 5. Insert encounter participant record (skip when no attending doctor)
        if (!attendingDoctorId.empty())
        {
            sql = R"(
INSERT INTO adm_encounter_participant
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, role_code, practitioner_id, status_enum, start_time, end_time)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, 'ATTENDING_DOCTOR', ?, 'ACTIVE', NOW(6), NULL)
)";
            params.clear();
            sp(params, generateUuid());
            sp(params, tenantId);
            sp(params, operUserId);
            sp(params, operUserId);
            sp(params, encounterId);
            sp(params, attendingDoctorId);

            if (sqlSession->executeUpdate(sql, params) < 1) {
                sqlSession->rollbackTransaction();
                return "INSERT_FAILED";
            }
        }

        // 6. Insert encounter location record (ward, status ACTIVE to match list/detail queries)
        if (!wardId.empty())
        {
            sql = R"(
INSERT INTO adm_encounter_location
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, location_id, form_enum, status_enum, start_time, end_time)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, ?, 'WARD', 'ACTIVE', NOW(6), NULL)
)";
            params.clear();
            sp(params, generateUuid());
            sp(params, tenantId);
            sp(params, operUserId);
            sp(params, operUserId);
            sp(params, encounterId);
            sp(params, wardId);

            if (sqlSession->executeUpdate(sql, params) < 1) {
                sqlSession->rollbackTransaction();
                return "INSERT_FAILED";
            }
        }

        // 7. Insert account record
        sql = R"(
INSERT INTO adm_account
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 patient_id, encounter_id, type_code, bus_no, name, balance,
 contract_no, billing_status_enum, encounter_flag, insutype, default_flag, status_enum)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, ?, 'PREPAY', ?, CONCAT(?,'住院预交账户'), ?,
 ?, 'UNSETTLE', 1, ?, 1, 'NORMAL')
)";
        params.clear();
        sp(params, accountId);
        sp(params, tenantId);
        sp(params, operUserId);
        sp(params, operUserId);
        sp(params, patientId);
        sp(params, encounterId);
        sp(params, accountBusNo);
        sp(params, name);
        sp(params, prepayAmount);
        sp(params, contractId);
        sp(params, feeNature);

        if (sqlSession->executeUpdate(sql, params) < 1) {
            sqlSession->rollbackTransaction();
            return "INSERT_FAILED";
        }

        // 8. Insert prepayment payment detail records (fin_payment_rec_detail)
        if (!paymentDetails.empty())
        {
            double runningBalance = 0.0;

            for (const AdmissionPaymentDetail& detail : paymentDetails)
            {
                double amount = 0.0;

                try
                {
                    amount = std::stod(detail.amount);
                }
                catch (const std::exception&)
                {
                    amount = 0.0;
                }

                if (amount < 0.0) amount = 0.0;

                // Payment record id doubles as reconciliation id (same pattern as PrepaymentDAO)
                std::string paymentRecId = generateUuid();

                sql = R"(
INSERT INTO fin_payment_rec_detail
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, version,
 reconciliation_id, target_enum,
 account_id, account_code,
 pay_enum, amount, charge_amount, return_amount,
 pay_trans_no, pay_trans_text, pay_trans_date,
 before_balance, after_balance, result_enum)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, 0,
 ?, 'PREPAYMENT',
 ?, ?,
 ?, ?, ?, 0,
 '', '', NOW(6),
 ?, ?, 'SUCCESS')
)";
                params.clear();
                sp(params, paymentRecId);
                sp(params, tenantId);
                sp(params, operUserId);
                sp(params, operUserId);
                sp(params, paymentRecId); // reconciliation_id = payment record id
                sp(params, accountId);
                sp(params, accountBusNo);
                sp(params, detail.payWay);
                SQLPARAMS_PUSH(params, "d", double, amount);
                SQLPARAMS_PUSH(params, "d", double, amount);
                SQLPARAMS_PUSH(params, "d", double, runningBalance);
                runningBalance += amount;
                SQLPARAMS_PUSH(params, "d", double, runningBalance);

                if (sqlSession->executeUpdate(sql, params) < 1) {
                    sqlSession->rollbackTransaction();
                    return "INSERT_FAILED";
                }
            }
        }

        // Commit transaction — all insert operations succeeded
        sqlSession->commitTransaction();
        return "";
    }
    catch (const std::exception& e)
    {
        sqlSession->rollbackTransaction();
        std::cerr << "AdmissionDAO::insertAdmission failed: " << e.what() << std::endl;
        return "INSERT_FAILED";
    }
}

std::string AdmissionDAO::deleteAdmission(
    const std::string& encounterId,
    const std::string& cancelReason,
    bool insuranceRevoked,
    const std::string& operUserId,
    const std::string& tenantId
)
{
    // Validate required fields
    if (encounterId.empty()) return "ENCOUNTER_NOT_FOUND";
    if (tenantId.empty()) return "DELETE_FAILED";

    sqlSession->beginTransaction();

    try
    {
        // Check the encounter exists and is cancelable
        std::string sql;
        SqlParams params;

        sql = R"(
SELECT id, status_enum
FROM adm_encounter
WHERE id = ? AND tenant_id = ? AND is_deleted = 0
)";
        params.clear();
        sp(params, encounterId);
        sp(params, tenantId);

        std::pair<std::string, std::string> encounter =
            sqlSession->executeQueryOne<std::pair<std::string, std::string>, EncounterStatusMapper>(
                sql, EncounterStatusMapper(), params
            );

        if (encounter.first.empty())
        {
            sqlSession->rollbackTransaction();
            return "ENCOUNTER_NOT_FOUND";
        }

        const std::string& statusEnum = encounter.second;
        if (statusEnum != "PLANNED" && statusEnum != "REGISTERED"
            && statusEnum != "IN_PROGRESS" && statusEnum != "INHOSPITAL")
        {
            sqlSession->rollbackTransaction();
            return "STATUS_NOT_CANCELABLE";
        }

        // 1. Persist cancel reason (and insurance revoked marker) into adm_encounter_reason
        sql = R"(
INSERT INTO adm_encounter_reason
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, use_code, reason_table, reason_id, description)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, 'CANCEL', 'adm_encounter', ?, ?)
)";
        params.clear();
        sp(params, generateUuid());
        sp(params, tenantId);
        sp(params, operUserId);
        sp(params, operUserId);
        sp(params, encounterId);
        sp(params, encounterId);
        sp(params, cancelReason);

        if (sqlSession->executeUpdate(sql, params) < 1) {
            sqlSession->rollbackTransaction();
            return "DELETE_FAILED";
        }

        if (insuranceRevoked)
        {
            sql = R"(
INSERT INTO adm_encounter_reason
(id, tenant_id, created_at, updated_at, created_by, updated_by,
 is_deleted, deleted_at, deleted_by, version,
 encounter_id, use_code, reason_table, reason_id, description)
VALUES
(?, ?, NOW(6), NOW(6), ?, ?,
 0, NULL, NULL, 1,
 ?, 'INSURANCE_REVOKED', 'adm_encounter', ?, ?)
)";
            params.clear();
            sp(params, generateUuid());
            sp(params, tenantId);
            sp(params, operUserId);
            sp(params, operUserId);
            sp(params, encounterId);
            sp(params, encounterId);
            sp(params, std::string("insurance revoked: ") + (cancelReason.empty() ? "-" : cancelReason));

            if (sqlSession->executeUpdate(sql, params) < 1) {
                sqlSession->rollbackTransaction();
                return "DELETE_FAILED";
            }
        }

        // Soft delete: set is_deleted = 1 on all related tables
        // Order follows foreign key dependencies (child tables first, parent last)

        // 2. Prepayment payment detail records (linked via account)
        sql = R"(
UPDATE fin_payment_rec_detail d
INNER JOIN adm_account a ON a.id = d.account_id
SET d.is_deleted = 1, d.deleted_at = NOW(6), d.deleted_by = ?, d.updated_at = NOW(6)
WHERE a.encounter_id = ? AND a.tenant_id = ? AND a.is_deleted = 0 AND d.is_deleted = 0
)";
        params.clear();
        sp(params, operUserId);
        sp(params, encounterId);
        sp(params, tenantId);
        sqlSession->executeUpdate(sql, params);

        // 3. Account table
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

        // 4. Encounter location table
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

        // 5. Encounter participant table
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

        // 6. Encounter diagnosis table
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

        // 7. Encounter reason table
        sql = R"(
UPDATE adm_encounter_reason
SET is_deleted = 1, deleted_at = NOW(6), deleted_by = ?, updated_at = NOW(6)
WHERE encounter_id = ? AND tenant_id = ? AND is_deleted = 0
)";
        params.clear();
        sp(params, operUserId);
        sp(params, encounterId);
        sp(params, tenantId);
        sqlSession->executeUpdate(sql, params);

        // 8. Encounter table (core encounter record)
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
        return "";
    }
    catch (const std::exception& e)
    {
        sqlSession->rollbackTransaction();
        std::cerr << "AdmissionDAO::deleteAdmission failed: " << e.what() << std::endl;
        return "DELETE_FAILED";
    }
}
