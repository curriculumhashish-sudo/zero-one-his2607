#ifndef _ORGANIZATIONTREEASSEMBLER_H_
#define _ORGANIZATIONTREEASSEMBLER_H_

#include <list>
#include <memory>

#include "tree/TreeUtil.h"
#include "domain/do/dept/adm_organizationDO.h"

class OrganizationTreeAssembler
{
public:
    /**
     * Converts the already filtered and ordered DO list into response nodes.
     * Only nodes with an empty parentId become roots. A node whose parent is
     * absent from the source cannot appear in the returned tree. Root and
     * sibling order follow the input order, which the DAO defines as
     * display_order ASC and id ASC. The input hierarchy must be acyclic.
     */
    template <typename TMapper>
    static std::list<std::shared_ptr<TreeNode>> build(
        const std::list<PtrAdmOrganizationDO>& source,
        const TMapper& mapper
    )
    {
        // TreeUtil discovers roots and attaches descendants recursively.
        return TreeUtil::listToTree<PtrAdmOrganizationDO>(source, mapper);
    }
};

#endif // !_ORGANIZATIONTREEASSEMBLER_H_
