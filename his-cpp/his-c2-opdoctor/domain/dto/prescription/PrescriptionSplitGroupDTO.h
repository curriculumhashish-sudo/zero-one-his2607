#pragma once

#ifndef _PRESCRIPTION_SPLIT_GROUP_DTO_H_
#define _PRESCRIPTION_SPLIT_GROUP_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 拆组医嘱请求对象
 *
 * 对用户勾选的一条或多条已并组医嘱执行拆组。
 *
 * 前置条件：
 * 1. 所有医嘱都必须处于未签发状态；
 * 2. 所有医嘱都必须已经并组。
 */
class PrescriptionSplitGroupDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionSplitGroupDTO, DTO);

    /**
     * 需要拆组的医嘱ID集合
     */
    API_DTO_FIELD_REQUIRE(
        List<String>,
        prescriptionIds,
        ZH_WORDS_GETTER(
            "prescription.field.splitPrescriptionIds"
        ),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif
