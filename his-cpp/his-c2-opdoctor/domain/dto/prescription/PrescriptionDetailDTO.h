#pragma once

#ifndef _PRESCRIPTIONDETAILDTO_H_
#define _PRESCRIPTIONDETAILDTO_H_

#include "PrescriptionIssueDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 处方详情DTO
 */
class PrescriptionDetailDTO : public PrescriptionIssueDTO
{
    DTO_INIT(PrescriptionDetailDTO, PrescriptionIssueDTO);

    // 处方ID
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("prescription.field.id"), true);

    // 处方状态
    API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("prescription.field.status"), true);

    // 创建人
    API_DTO_FIELD_REQUIRE(String, createBy, ZH_WORDS_GETTER("prescription.field.createBy"), true);

    // 更新人
    API_DTO_FIELD_REQUIRE(String, updateBy, ZH_WORDS_GETTER("prescription.field.updateBy"), false);
};

/**
 * 分页DTO
 */
class PrescriptionDetailPageDTO : public PageDTO<PrescriptionDetailDTO::Wrapper>
{
    DTO_INIT(PrescriptionDetailPageDTO, PageDTO<PrescriptionDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PRESCRIPTIONDETAILDTO_H_