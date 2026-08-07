#pragma once
#ifndef _WARDTREENODE_VO_H_
#define _WARDTREENODE_VO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class WardTreeNodeVO : public oatpp::DTO
{
    DTO_INIT(WardTreeNodeVO, DTO);

    DTO_FIELD(String, id);
    DTO_FIELD_INFO(id) {
        info->description = ZH_WORDS_GETTER("ward.field.id");
    }

    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("ward.field.name");
    }

    DTO_FIELD(String, formEnum);
    DTO_FIELD_INFO(formEnum) {
        info->description = ZH_WORDS_GETTER("ward.field.formEnum");
    }

    DTO_FIELD(String, parentId);
    DTO_FIELD_INFO(parentId) {
        info->description = ZH_WORDS_GETTER("ward.field.parentId");
    }

    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("ward.field.statusEnum");
    }

    DTO_FIELD(String, operationalEnum);
    DTO_FIELD_INFO(operationalEnum) {
        info->description = ZH_WORDS_GETTER("ward.field.operationalEnum");
    }

    DTO_FIELD(Int32, displayOrder);
    DTO_FIELD_INFO(displayOrder) {
        info->description = ZH_WORDS_GETTER("ward.field.displayOrder");
    }

   

    // 业务编号/位置编码
    DTO_FIELD(String, busNo);
    DTO_FIELD_INFO(busNo) {
        info->description = ZH_WORDS_GETTER("ward.field.busNo");
    }

    // 所属机构ID
    DTO_FIELD(String, organizationId);
    DTO_FIELD_INFO(organizationId) {
        info->description = ZH_WORDS_GETTER("ward.field.organizationId");
    }

    // 拼音检索码
    DTO_FIELD(String, pyStr);
    DTO_FIELD_INFO(pyStr) {
        info->description = ZH_WORDS_GETTER("ward.field.pyStr");
    }

    // 五笔检索码
    DTO_FIELD(String, wbStr);
    DTO_FIELD_INFO(wbStr) {
        info->description = ZH_WORDS_GETTER("ward.field.wbStr");
    }

    // 扩展类型配置 JSON
    DTO_FIELD(String, typeJson);
    DTO_FIELD_INFO(typeJson) {
        info->description = ZH_WORDS_GETTER("ward.field.typeJson");
    }

    // 乐观锁版本号
    DTO_FIELD(Int32, version);
    DTO_FIELD_INFO(version) {
        info->description = ZH_WORDS_GETTER("ward.field.version");
    }

    // 子节点列表
    DTO_FIELD(List<WardTreeNodeVO::Wrapper>, children);
    DTO_FIELD_INFO(children) {
        info->description = ZH_WORDS_GETTER("ward.field.children");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif