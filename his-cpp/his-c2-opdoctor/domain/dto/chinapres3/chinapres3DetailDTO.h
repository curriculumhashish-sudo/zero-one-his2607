#pragma once

#ifndef _chinapres3DETAILDTO_H_
#define _chinapres3DETAILDTO_H_

#include "chinapres3IssueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 处方详情DTO
 */
class chinapres3DetailDTO : public chinapres3IssueDTO
{
    DTO_INIT(chinapres3DetailDTO, chinapres3IssueDTO);

    // 处方ID
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("chinapres3.field.chinapres3Id"), true);

    // 处方状态
    API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("chinapres3.field.status"), true);

    // 创建人
    API_DTO_FIELD_REQUIRE(String, createBy, ZH_WORDS_GETTER("chinapres3.field.createBy"), true);

    // 更新人
    API_DTO_FIELD_REQUIRE(String, updateBy, ZH_WORDS_GETTER("chinapres3.field.updateBy"), false);
};

/**
 * 分页DTO
 */
class chinapres3DetailPageDTO : public PageDTO<chinapres3DetailDTO::Wrapper>
{
    DTO_INIT(chinapres3DetailPageDTO, PageDTO<chinapres3DetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_chinapres3DETAILDTO_H_