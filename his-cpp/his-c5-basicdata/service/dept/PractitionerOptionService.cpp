#include "stdafx.h"
#include "PractitionerOptionService.h"

#include "dao/dept/PractitionerOptionDAO.h"

PractitionerOptionQueryResult PractitionerOptionService::getDoctorOptions(
    const PayloadDTO& payload
)
{
    PractitionerOptionQueryResult result;
    PractitionerOptionDAO dao;
    const std::string tenantId =
        dao.selectTenantIdByUserId(payload.getId());
    if (tenantId.empty())
    {
        return result;
    }

    result.tenantFound = true;
    result.options = dao.selectDoctorOptions(tenantId);
    return result;
}
