#include "stdafx.h"
#include "MedicationRequestDAO.h"
#include "MedicationRequestMapper.h"

PtrMedicationRequestDO MedicationRequestDAO::selectById(const std::string& id)
{
    // 加载常规医嘱查询所需字段。
    std::string sql =
        "SELECT id, status_enum, status_changed_time, "
        "performer_check_id, check_time, updated_by, updated_at "
        "FROM med_medication_request "
        "WHERE id=? AND is_deleted=0";

    return sqlSession->executeQueryOne<PtrMedicationRequestDO>(
        sql, PtrMedicationRequestMapper(), "%s", id);
}

PtrMedicationRequestDO MedicationRequestDAO::selectByIdForUpdate(const std::string& id)
{
    // 记录锁定时仅需状态校验字段。
    std::string sql =
        "SELECT id, status_enum "
        "FROM med_medication_request "
        "WHERE id=? AND is_deleted=0 FOR UPDATE";

    return sqlSession->executeQueryOne<PtrMedicationRequestDO>(
        sql, PtrMedicationRequestStatusMapper(), "%s", id);
}
