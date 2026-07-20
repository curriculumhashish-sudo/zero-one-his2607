#pragma once

#ifndef _PRESCRIPTION_CONTROLLER_H_
#define _PRESCRIPTION_CONTROLLER_H_

#include "../../domain/GlobalInclude.h"

#include "../../domain/dto/prescription/PrescriptionBatchDeleteDTO.h"
#include "../../domain/dto/prescription/PrescriptionMergeGroupDTO.h"
#include "../../domain/dto/prescription/PrescriptionSplitGroupDTO.h"
#include "../../domain/vo/prescription/PrescriptionOperationVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("prescription.tags.tag")

/**
 * 医嘱管理接口
 */
class PrescriptionController
    : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PrescriptionController);

public:

    /**
     * 删除医嘱，支持批量操作
     *
     * 只允许删除未签发状态的医嘱。
     */
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER(
            "prescription.batchDelete.summary"
        ),
        batchDeletePrescription,
        PrescriptionDeleteJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_DEL,
        "/prescription/batch",
        batchDeletePrescription,
        BODY_DTO(PrescriptionBatchDeleteDTO::Wrapper, dto),
        execBatchDeletePrescription(dto)
    );

    /**
     * 并组医嘱
     *
     * 选择多条未签发医嘱进行并组。
     */
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER(
            "prescription.mergeGroup.summary"
        ),
        mergePrescriptionGroup,
        PrescriptionMergeJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/prescription/group/merge",
        mergePrescriptionGroup,
        BODY_DTO(PrescriptionMergeGroupDTO::Wrapper, dto),
        execMergePrescriptionGroup(dto)
    );

    /**
     * 拆组医嘱
     *
     * 选择一条或多条已并组且未签发的医嘱进行拆组。
     */
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER(
            "prescription.splitGroup.summary"
        ),
        splitPrescriptionGroup,
        PrescriptionSplitJsonVO::Wrapper,
        API_TAG
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/prescription/group/split",
        splitPrescriptionGroup,
        BODY_DTO(PrescriptionSplitGroupDTO::Wrapper, dto),
        execSplitPrescriptionGroup(dto)
    );

private:

    PrescriptionDeleteJsonVO::Wrapper
        execBatchDeletePrescription(
            const PrescriptionBatchDeleteDTO::Wrapper& dto
        );

    PrescriptionMergeJsonVO::Wrapper
        execMergePrescriptionGroup(
            const PrescriptionMergeGroupDTO::Wrapper& dto
        );

    PrescriptionSplitJsonVO::Wrapper
        execSplitPrescriptionGroup(
            const PrescriptionSplitGroupDTO::Wrapper& dto
        );
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif
