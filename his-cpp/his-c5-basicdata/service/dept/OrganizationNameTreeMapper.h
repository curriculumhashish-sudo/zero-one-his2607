#ifndef _ORGANIZATIONNAMETREEMAPPER_H_
#define _ORGANIZATIONNAMETREEMAPPER_H_

#include "tree/TreeNodeMapper.h"
#include "domain/do/dept/adm_organizationDO.h"
#include "domain/dto/dept/OrganizationNameTreeDTO.h"

class OrganizationNameTreeMapper
    : public TreeNodeMapper<PtrAdmOrganizationDO>
{
public:
    std::shared_ptr<TreeNode> objectMapper(
        PtrAdmOrganizationDO source
    ) const override
    {
        auto result = std::make_shared<OrganizationNameTreeDTO>();

        result->_id(source->getId());
        result->_pid(source->getParentId());

        result->id = source->getId();
        result->name = source->getName();

        return result;
    }
};

#endif // !_ORGANIZATIONNAMETREEMAPPER_H_
