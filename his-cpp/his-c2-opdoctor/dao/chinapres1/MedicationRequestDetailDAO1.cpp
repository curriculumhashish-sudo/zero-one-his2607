#include "stdafx.h"
#include "MedicationRequestDetailDAO1.h"
#include "MedicationRequestDetailMapper1.h"

std::list<PtrMedicationRequestDetailDO> MedicationRequestDetailDAO1::selectByRequestId(const string& requestId)
{
    string sql = "SELECT id, request_id, medication_id, quantity, unit_code, dose, dose_unit_code, sort_no "
        "FROM med_medication_request_detail "
        "WHERE request_id = ? AND is_deleted = 0 "
        "ORDER BY sort_no";
    return sqlSession->executeQuery<PtrMedicationRequestDetailDO>(sql, MedicationRequestDetailMapper1(), "%s", requestId);
}