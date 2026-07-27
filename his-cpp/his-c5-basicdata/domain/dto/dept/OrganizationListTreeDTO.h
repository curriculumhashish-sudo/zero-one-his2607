#ifndef _ORGANIZATIONLISTTREEDTO_H_
#define _ORGANIZATIONLISTTREEDTO_H_

#include "../../GlobalInclude.h"
#include "tree/TreeNode.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OrganizationListTreeDTO : public oatpp::DTO, public TreeNode
{
    DTO_INIT(OrganizationListTreeDTO, DTO);

    API_DTO_FIELD_REQUIRE(oatpp::String, id, ZH_WORDS_GETTER("dept.organizationQuery.fields.id"), true);
    API_DTO_FIELD_REQUIRE(oatpp::String, name, ZH_WORDS_GETTER("dept.organizationQuery.fields.name"), true);
    API_DTO_FIELD_DEFAULT(oatpp::String, busNo, ZH_WORDS_GETTER("dept.organizationQuery.fields.busNo"));
    API_DTO_FIELD_DEFAULT(oatpp::String, typeEnum, ZH_WORDS_GETTER("dept.organizationQuery.fields.typeEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, classEnum, ZH_WORDS_GETTER("dept.organizationQuery.fields.classEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::String, statusEnum, ZH_WORDS_GETTER("dept.organizationQuery.fields.statusEnum"));
    API_DTO_FIELD_DEFAULT(oatpp::Int32, displayOrder, ZH_WORDS_GETTER("dept.organizationQuery.fields.displayOrder"));

    API_DTO_FIELD(
        oatpp::List<OrganizationListTreeDTO::Wrapper>,
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
                std::dynamic_pointer_cast<OrganizationListTreeDTO>(child),
                Wrapper::Class::getType()
            )
        );
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_ORGANIZATIONLISTTREEDTO_H_
