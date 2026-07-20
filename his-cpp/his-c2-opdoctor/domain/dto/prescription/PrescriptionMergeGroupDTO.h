#pragma once

#ifndef _PRESCRIPTION_MERGE_GROUP_DTO_H_
#define _PRESCRIPTION_MERGE_GROUP_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 并组医嘱请求对象
 *
 * 将用户勾选的多条未签发医嘱合并为一组。
 * 至少需要选择两条医嘱。
 *
 * groupNo 应由服务端生成，前端不需要传入。
 */
class PrescriptionMergeGroupDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionMergeGroupDTO, DTO);

    /**
     * 需要并组的医嘱ID集合
     */
    API_DTO_FIELD_REQUIRE(
        List<String>,
        prescriptionIds,
        ZH_WORDS_GETTER(
            "prescription.field.mergePrescriptionIds"
        ),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif
