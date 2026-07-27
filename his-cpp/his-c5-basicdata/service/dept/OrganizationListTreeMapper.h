#ifndef _ORGANIZATIONLISTTREEMAPPER_H_
#define _ORGANIZATIONLISTTREEMAPPER_H_

#include "tree/TreeNodeMapper.h"
#include "domain/do/dept/adm_organizationDO.h"
#include "domain/dto/dept/OrganizationListTreeDTO.h"

class OrganizationListTreeMapper
    : public TreeNodeMapper<PtrAdmOrganizationDO>
{
public:
    std::shared_ptr<TreeNode> objectMapper(
        PtrAdmOrganizationDO source
    ) const override
    {
        auto result = std::make_shared<OrganizationListTreeDTO>();

        result->_id(source->getId());
        result->_pid(source->getParentId());

        result->id = source->getId();
        result->name = source->getName();
        result->busNo = source->getBusNo();
        result->typeEnum = source->getTypeEnum();
        result->classEnum = source->getClassEnum();
        result->statusEnum = source->getStatusEnum();
        result->displayOrder = source->getDisplayOrder();

        return result;
    }
};

#endif // !_ORGANIZATIONLISTTREEMAPPER_H_
