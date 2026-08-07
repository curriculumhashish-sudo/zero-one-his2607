#ifndef _PRACTITIONER_OPTION_DAO_H_
#define _PRACTITIONER_OPTION_DAO_H_

#include "BaseDAO.h"
#include "domain/dto/dept/PractitionerOptionDTO.h"

#include <list>
#include <string>

class PractitionerOptionDAO : public BaseDAO
{
public:
    /**
     * Resolve the tenant of the authenticated user from the latest login
     * session. An empty string means that no tenant is available.
     */
    std::string selectTenantIdByUserId(const std::string& userId);

    /**
     * Return active practitioners assigned to the ROLE_DOCTOR role for a
     * tenant. DISTINCT prevents duplicate options from multiple assignments.
     */
    std::list<PractitionerOptionDTO::Wrapper> selectDoctorOptions(
        const std::string& tenantId
    );
};

#endif // !_PRACTITIONER_OPTION_DAO_H_
