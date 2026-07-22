#pragma once

#ifndef _PRESCRIPTION3_BATCH_DELETE_DTO_H_
#define _PRESCRIPTION3_BATCH_DELETE_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 删除医嘱请求对象
 *
 * 支持删除一条或多条未签发医嘱。
 */
class Prescription3BatchDeleteDTO : public oatpp::DTO
{
    DTO_INIT(
        Prescription3BatchDeleteDTO,
        DTO
    );

    /**
     * 需要删除的医嘱ID集合
     */
    API_DTO_FIELD_REQUIRE(
        List<String>,
        prescriptionIds,
        ZH_WORDS_GETTER(
            "prescription3.field.deletePrescriptionIds"
        ),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif