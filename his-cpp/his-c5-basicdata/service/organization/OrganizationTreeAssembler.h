#ifndef _ORGANIZATIONTREEASSEMBLER_H_
#define _ORGANIZATIONTREEASSEMBLER_H_

#include <list>
#include <memory>

#include "tree/TreeUtil.h"
#include "domain/do/organization/adm_organizationDO.h"

class OrganizationTreeAssembler
{
public:
    template <typename TMapper>
    static std::list<std::shared_ptr<TreeNode>> build(
        const std::list<PtrAdmOrganizationDO>& source,
        const TMapper& mapper
    )
    {
        // TreeUtil returns every root node and attaches descendants recursively.
        return TreeUtil::listToTree<PtrAdmOrganizationDO>(source, mapper);
    }
};

#endif // !_ORGANIZATIONTREEASSEMBLER_H_