#ifndef _ORGANIZATIONNAMETREEDTO_H_
#define _ORGANIZATIONNAMETREEDTO_H_

#include "../../GlobalInclude.h"
#include "tree/TreeNode.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * One node in the selectable organization name tree.
 *
 * id is serialized as a JSON string to preserve large identifiers. children
 * contains direct child nodes; a leaf node exposes an empty children array.
 * The endpoint may return multiple root nodes in its top-level data array.
 */
class OrganizationNameTreeDTO : public oatpp::DTO, public TreeNode
{
    DTO_INIT(OrganizationNameTreeDTO, DTO);

    API_DTO_FIELD_REQUIRE(oatpp::String, id, ZH_WORDS_GETTER("dept.organizationQuery.fields.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("dept.organizationQuery.fields.name"), true);

    API_DTO_FIELD(
        oatpp::List<OrganizationNameTreeDTO::Wrapper>,
        children,
        ZH_WORDS_GETTER("dept.organizationQuery.fields.children"),
        false,
        {}
    );

public:
    void addChild(std::shared_ptr<TreeNode> child) override
    {
        children->push_back(
            Wrapper(
                std::dynamic_pointer_cast<OrganizationNameTreeDTO>(child),
                Wrapper::Class::getType()
            )
        );
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONNAMETREEDTO_H_
