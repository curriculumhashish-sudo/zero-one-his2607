#ifndef _ORGANIZATIONLISTTREEDTO_H_
#define _ORGANIZATIONLISTTREEDTO_H_

#include "../../GlobalInclude.h"
#include "tree/TreeNode.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationListTreeDTO : public oatpp::DTO, public TreeNode
{
    DTO_INIT(OrganizationListTreeDTO, DTO);

    API_DTO_FIELD_REQUIRE(String, id, "Organization id", true);
    API_DTO_FIELD_REQUIRE(String, name, "Organization name", true);
    API_DTO_FIELD_DEFAULT(String, type_enum, "Organization type");
    API_DTO_FIELD_DEFAULT(String, class_enum, "Organization category");
    API_DTO_FIELD_DEFAULT(String, status_enum, "Organization status");

    API_DTO_FIELD(
        List<OrganizationListTreeDTO::Wrapper>,
        children,
        "Child organizations",
        false,
        {}
    );

public:
    void addChild(std::shared_ptr<TreeNode> child) override
    {
        children->push_back(
            Wrapper(
                std::dynamic_pointer_cast<OrganizationListTreeDTO>(child),
                Wrapper::Class::getType()
            )
        );
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONLISTTREEDTO_H_