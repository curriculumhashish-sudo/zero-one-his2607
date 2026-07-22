#pragma once

#ifndef _PRESCRIPTION3_MERGE_GROUP_DTO_H_
#define _PRESCRIPTION3_MERGE_GROUP_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 并组医嘱请求对象
 *
 * 将用户勾选的多条未签发医嘱合并为一个医嘱组。
 * 至少需要选择两条医嘱。
 * 医嘱组号由服务端生成。
 */
class Prescription3MergeGroupDTO : public oatpp::DTO
{
    DTO_INIT(
        Prescription3MergeGroupDTO,
        DTO
    );

    /**
     * 需要并组的医嘱ID集合
     */
    API_DTO_FIELD_REQUIRE(
        List<String>,
        prescriptionIds,
        ZH_WORDS_GETTER(
            "prescription3.field.mergePrescriptionIds"
        ),
        true
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif