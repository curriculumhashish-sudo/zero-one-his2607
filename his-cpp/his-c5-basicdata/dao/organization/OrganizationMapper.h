#ifndef _ORGANIZATIONMAPPER_H_
#define _ORGANIZATIONMAPPER_H_

#include "Mapper.h"
#include "domain/do/organization/adm_organizationDO.h"

class OrganizationMapper : public Mapper<PtrAdmOrganizationDO>
{
private:
    // put NULL to normal .
    std::string readNullableString(ResultSet* resultSet, const std::string& column) const;

public:
    PtrOrganizationDO mapper(ResultSet* resultSet) const override;
};

#endif // !_ORGANIZATIONMAPPER_H_