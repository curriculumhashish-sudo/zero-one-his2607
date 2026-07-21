#ifndef _ORGANIZATIONLISTTREEDTO_H_
#define _ORGANIZATIONLISTTREEDTO_H_

#include "../../GlobalInclude.h"
#include "tree/TreeNode.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationListTreeDTO : public oatpp::DTO, public TreeNode
{
    DTO_INIT(OrganizationListTreeDTO, DTO);

    API_DTO_FIELD_REQUIRE(oatpp::String, id, ZH_WORDS_GETTER("organization.fields.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("organization.fields.name"), true);
    API_DTO_FIELD_DEFAULT(oatpp::String, type_enum, ZH_WORDS_GETTER("organization.fields.typeEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, class_enum, ZH_WORDS_GETTER("organization.fields.classEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, status_enum, ZH_WORDS_GETTER("organization.fields.statusEnum"));

    API_DTO_FIELD(
        oatpp::List<OrganizationListTreeDTO::Wrapper>,
        children,
        ZH_WORDS_GETTER("organization.fields.children"),
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
