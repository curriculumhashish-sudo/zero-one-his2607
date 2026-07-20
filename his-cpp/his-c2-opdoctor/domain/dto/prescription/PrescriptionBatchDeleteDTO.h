#pragma once

#ifndef _PRESCRIPTION_BATCH_DELETE_DTO_H_
#define _PRESCRIPTION_BATCH_DELETE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 删除医嘱请求对象
 *
 * 支持删除一条或多条医嘱。
 * 只有处于未签发（草稿）状态的医嘱才能删除。
 */
class PrescriptionBatchDeleteDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionBatchDeleteDTO, DTO);

    /**
     * 需要删除的医嘱ID集合
     */
    API_DTO_FIELD_REQUIRE(
        List<String>,
        prescriptionIds,
        ZH_WORDS_GETTER(
            "prescription.field.deletePrescriptionIds"
        ),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif