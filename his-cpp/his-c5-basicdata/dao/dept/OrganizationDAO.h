#ifndef _ORGANIZATIONDAO_H_
#define _ORGANIZATIONDAO_H_

#include "BaseDAO.h"
#include "domain/do/dept/adm_organizationDO.h"

class OrganizationDAO : public BaseDAO
{
public:
    std::list<PtrAdmOrganizationDO> selectAllNotDeleted();
    PtrAdmOrganizationDO selectById(const std::string& id);
};

#endif // !_ORGANIZATIONDAO_H_
