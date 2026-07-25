#ifndef _ORGANIZATIONLISTTREEMAPPER_H_
#define _ORGANIZATIONLISTTREEMAPPER_H_

#include "tree/TreeNodeMapper.h"
#include "domain/do/organization/adm_organizationDO.h"
#include "domain/dto/organization/OrganizationListTreeDTO.h"

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
        result->_pid(source->getParent_Id());

        result->id = source->getId();
        result->name = source->getName();
        result->type_enum = source->getType_Enum();
        result->class_enum = source->getClass_Enum();
        result->status_enum = source->getStatus_Enum();

        return result;
    }
};

#endif // !_ORGANIZATIONLISTTREEMAPPER_H_