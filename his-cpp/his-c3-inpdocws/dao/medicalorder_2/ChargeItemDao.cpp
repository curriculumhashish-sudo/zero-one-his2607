#include "stdafx.h"
#include "ChargeItemDAO.h"
#include "ChargeItemMapper.h"
#include "domain/do/medicalorder_2/AdmChargeItemDO.h"


//ͣ��ǰ����շ�״̬- δ�շ� �� ��ͣ- ���շ� �� ǰ�˵�����ʾ�˷�
std::list<PtrAdmChargeItemDO> ChargeItemDAO::selectByService(
    const std::string& serviceTable,
    const std::string& serviceId)
{
    if (serviceTable.empty() || serviceId.empty()) {
        return {};
    }

    std::string sql =
        "SELECT id, parent_id, service_table, service_id, status_enum "
        "FROM adm_charge_item "
        "WHERE service_table = ? AND service_id = ? AND is_deleted = 0";

    SqlParams params;
    SQLPARAMS_PUSH(params, "s", std::string, serviceTable);
    SQLPARAMS_PUSH(params, "s", std::string, serviceId);

    ChargeItemMapper mapper;
    return sqlSession->executeQuery<PtrAdmChargeItemDO>(sql, mapper, params);
}