#pragma once

#ifndef _PRESCRIPTION3_SPLIT_GROUP_DTO_H_
#define _PRESCRIPTION3_SPLIT_GROUP_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 拆组医嘱请求对象
 *
 * 对用户勾选的一条或多条医嘱执行拆组。
 *
 * 前置条件：
 * 1. 所选医嘱必须处于未签发状态；
 * 2. 所选医嘱必须已经并组。
 */
class Prescription3SplitGroupDTO : public oatpp::DTO
{
    DTO_INIT(
        Prescription3SplitGroupDTO,
        DTO
    );

    /**
     * 需要拆组的医嘱ID集合
     */
    API_DTO_FIELD_REQUIRE(
        List<String>,
        prescriptionIds,
        ZH_WORDS_GETTER(
            "prescription3.field.splitPrescriptionIds"
        ),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif