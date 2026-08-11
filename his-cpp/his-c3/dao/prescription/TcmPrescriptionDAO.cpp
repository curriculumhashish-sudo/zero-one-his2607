/*
 Copyright Zero One Star. All rights reserved.

 Implements database operations used by TCM prescription actions.

 @Author: nick1484065161
 @Date: 2026/07/23 00:00:00

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
#include "TcmPrescriptionDAO.h"
#include "TcmPrescriptionMapper.h"

namespace
{
    // Keep this SELECT column order aligned with PtrMedMedicationRequestMapper.
    const std::string SELECT_MEDICATION_REQUEST =
        "SELECT id, tenant_id, version, IFNULL(prescription_no, ''), "
        "IFNULL(group_id, ''), status_enum, IFNULL(status_reason, 0), "
        "IFNULL(status_changed_time, ''), patient_id, encounter_id, "
        "IFNULL(effective_dose_end, ''), therapy_enum, IFNULL(tcm_flag, 0), "
        "IFNULL(content_json, ''), "
        "IF(JSON_VALID(content_json), "
        "IFNULL(JSON_UNQUOTE(JSON_EXTRACT(content_json, '$.prescriptionGroupKey')), ''), ''), "
        "is_deleted, IFNULL(updated_by, ''), IFNULL(updated_at, '') "
        "FROM zo_his.med_medication_request ";
}

PtrMedMedicationRequestDO TcmPrescriptionDAO::selectByIdForUpdate(
    const std::string& id,
    const std::string& tenantId
)
{
    const std::string sql =
        SELECT_MEDICATION_REQUEST +
        "WHERE id = ? AND tenant_id = ? FOR UPDATE";

    // Bind string parameters in SQL placeholder order.
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, id);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);

    return sqlSession->executeQueryOne<PtrMedMedicationRequestDO>(
        sql,
        PtrMedMedicationRequestMapper(),
        params
    );
}

std::list<PtrMedMedicationRequestDO>
TcmPrescriptionDAO::selectByPrescriptionNoForUpdate(
    const std::string& prescriptionNo,
    const std::string& tenantId
)
{
    const std::string sql =
        SELECT_MEDICATION_REQUEST +
        "WHERE prescription_no = ? AND tenant_id = ? "
        "ORDER BY id FOR UPDATE";

    // Bind string parameters in SQL placeholder order.
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, prescriptionNo);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);

    return sqlSession->executeQuery<PtrMedMedicationRequestDO>(
        sql,
        PtrMedMedicationRequestMapper(),
        params
    );
}

int TcmPrescriptionDAO::countWithdrawalBlockingRecords(
    const std::string& requestId,
    const std::string& prescriptionNo,
    const std::string& tenantId
)
{
    const std::string sql =
        "SELECT CASE WHEN "
        "EXISTS (SELECT 1 FROM zo_his.med_medication_dispense "
        "WHERE med_req_id = ? AND tenant_id = ? AND is_deleted = 0) "
        "OR EXISTS (SELECT 1 FROM zo_his.adm_charge_item "
        "WHERE (service_id = ? OR prescription_no = ?) "
        "AND tenant_id = ? AND is_deleted = 0) "
        "OR EXISTS (SELECT 1 FROM zo_his.cln_procedure "
        "WHERE request_id = ? AND request_table = 'med_medication_request' "
        "AND tenant_id = ? AND is_deleted = 0) "
        "THEN 1 ELSE 0 END";

    // Bind parameters for dispensing, charging, and execution checks.
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, requestId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, requestId);
    SQLPARAMS_PUSH(params, "s", std::string, prescriptionNo);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);
    SQLPARAMS_PUSH(params, "s", std::string, requestId);
    SQLPARAMS_PUSH(params, "s", std::string, tenantId);

    return sqlSession->executeQueryOne<int>(
        sql,
        TcmIntegerMapper(),
        params
    );
}

int TcmPrescriptionDAO::updateForSign(
    const MedMedicationRequestDO& current,
    const std::string& prescriptionNo,
    const std::string& targetStatus,
    const std::string& changedTime,
    const std::string& operatorId,
    const std::string& expectedStatus
)
{
    const std::string sql =
        "UPDATE zo_his.med_medication_request "
        "SET prescription_no = ?, status_enum = ?, status_reason = NULL, "
        "status_changed_time = ?, updated_by = ?, updated_at = ?, "
        "version = version + 1 "
        "WHERE id = ? AND tenant_id = ? AND patient_id = ? AND encounter_id = ? "
        "AND is_deleted = 0 AND tcm_flag = 1 "
        "AND (prescription_no IS NULL OR prescription_no = '') "
        "AND status_enum = ? AND version = ?";

    // Bind SET values before the guarded WHERE values.
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, prescriptionNo);
    SQLPARAMS_PUSH(params, "s", std::string, targetStatus);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, current.getId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getTenantId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getPatientId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getEncounterId());
    SQLPARAMS_PUSH(params, "s", std::string, expectedStatus);
    SQLPARAMS_PUSH(params, "i", int, current.getVersion());

    return sqlSession->executeUpdate(sql, params);
}

int TcmPrescriptionDAO::updateForWithdraw(
    const MedMedicationRequestDO& current,
    const std::string& targetStatus,
    const std::string& changedTime,
    const std::string& operatorId,
    const std::string& expectedStatus
)
{
    const std::string sql =
        "UPDATE zo_his.med_medication_request "
        "SET prescription_no = NULL, status_enum = ?, status_reason = NULL, "
        "status_changed_time = ?, effective_dose_end = NULL, "
        "updated_by = ?, updated_at = ?, version = version + 1 "
        "WHERE id = ? AND tenant_id = ? AND patient_id = ? AND encounter_id = ? "
        "AND prescription_no = ? AND is_deleted = 0 AND tcm_flag = 1 "
        "AND status_enum = ? AND version = ?";

    // Bind SET values before the guarded WHERE values.
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, targetStatus);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, current.getId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getTenantId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getPatientId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getEncounterId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getPrescriptionNo());
    SQLPARAMS_PUSH(params, "s", std::string, expectedStatus);
    SQLPARAMS_PUSH(params, "i", int, current.getVersion());

    return sqlSession->executeUpdate(sql, params);
}

int TcmPrescriptionDAO::updateForStop(
    const MedMedicationRequestDO& current,
    const std::string& targetStatus,
    const std::string& stopTime,
    const std::string& changedTime,
    const std::string& operatorId,
    const std::string& expectedStatus
)
{
    const std::string sql =
        "UPDATE zo_his.med_medication_request "
        "SET status_enum = ?, status_reason = NULL, status_changed_time = ?, "
        "effective_dose_end = ?, updated_by = ?, updated_at = ?, "
        "version = version + 1 "
        "WHERE id = ? AND tenant_id = ? AND patient_id = ? AND encounter_id = ? "
        "AND prescription_no = ? AND is_deleted = 0 AND tcm_flag = 1 "
        "AND status_enum = ? AND version = ?";

    // Bind SET values before the guarded WHERE values.
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, targetStatus);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, stopTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, current.getId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getTenantId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getPatientId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getEncounterId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getPrescriptionNo());
    SQLPARAMS_PUSH(params, "s", std::string, expectedStatus);
    SQLPARAMS_PUSH(params, "i", int, current.getVersion());

    return sqlSession->executeUpdate(sql, params);
}

int TcmPrescriptionDAO::updateForCancelStop(
    const MedMedicationRequestDO& current,
    const std::string& targetStatus,
    const std::string& changedTime,
    const std::string& operatorId,
    const std::string& expectedStatus
)
{
    const std::string sql =
        "UPDATE zo_his.med_medication_request "
        "SET status_enum = ?, status_reason = NULL, status_changed_time = ?, "
        "effective_dose_end = NULL, updated_by = ?, updated_at = ?, "
        "version = version + 1 "
        "WHERE id = ? AND tenant_id = ? AND patient_id = ? AND encounter_id = ? "
        "AND prescription_no = ? AND is_deleted = 0 AND tcm_flag = 1 "
        "AND status_enum = ? AND version = ?";

    // Bind SET values before the guarded WHERE values.
    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, targetStatus);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, operatorId);
    SQLPARAMS_PUSH(params, "s", std::string, changedTime);
    SQLPARAMS_PUSH(params, "s", std::string, current.getId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getTenantId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getPatientId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getEncounterId());
    SQLPARAMS_PUSH(params, "s", std::string, current.getPrescriptionNo());
    SQLPARAMS_PUSH(params, "s", std::string, expectedStatus);
    SQLPARAMS_PUSH(params, "i", int, current.getVersion());

    return sqlSession->executeUpdate(sql, params);
}
