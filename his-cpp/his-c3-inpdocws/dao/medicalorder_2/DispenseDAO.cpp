#include "stdafx.h"
#include "DispenseDAO.h"
#include "DispenseMapper.h"
#include "domain/do/medicalorder_2/MedMedicationDispenseDO.h"

//selectByMedReqId �� ��ҩƷ����ID�鷢ҩ��¼
PtrMedMedicationDispenseDO DispenseDAO::selectByMedReqId(
    const std::string& medReqId)
{
    if (medReqId.empty()) return nullptr;

    std::string sql =
        "SELECT id, med_req_id, status_enum, dispense_quantity "
        "FROM med_medication_dispense "
        "WHERE med_req_id = ? AND is_deleted = 0 "
        "LIMIT 1";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, medReqId);

    DispenseMapper mapper;
    return sqlSession->executeQueryOne<PtrMedMedicationDispenseDO>(sql, mapper, params);
}

//hasDispensedRecord �� ���Ƿ������ѷ�ҩ��¼
bool DispenseDAO::hasDispensedRecord(const std::string& medReqId)
{
    if (medReqId.empty()) return false;

    std::string sql =
        "SELECT COUNT(1) AS cnt "
        "FROM med_medication_dispense "
        "WHERE med_req_id = ? "
        "  AND status_enum = 'dispensed' "
        "  AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, medReqId);

    return sqlSession->executeQueryNumerical(sql, params) > 0;
}