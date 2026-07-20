#pragma once

#ifndef _PRESCRIPTION_OPERATION_VO_H_
#define _PRESCRIPTION_OPERATION_VO_H_

#include "../../GlobalInclude.h"
#include "../../dto/prescription/PrescriptionOperationResultDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PrescriptionDeleteJsonVO
    : public JsonVO<PrescriptionDeleteResultDTO::Wrapper>
{
    DTO_INIT(
        PrescriptionDeleteJsonVO,
        JsonVO<PrescriptionDeleteResultDTO::Wrapper>
    );

public:
    PrescriptionDeleteJsonVO()
    {
        data = PrescriptionDeleteResultDTO::createShared();
    }
};

class PrescriptionMergeJsonVO
    : public JsonVO<PrescriptionMergeResultDTO::Wrapper>
{
    DTO_INIT(
        PrescriptionMergeJsonVO,
        JsonVO<PrescriptionMergeResultDTO::Wrapper>
    );

public:
    PrescriptionMergeJsonVO()
    {
        data = PrescriptionMergeResultDTO::createShared();
    }
};

class PrescriptionSplitJsonVO
    : public JsonVO<PrescriptionSplitResultDTO::Wrapper>
{
    DTO_INIT(
        PrescriptionSplitJsonVO,
        JsonVO<PrescriptionSplitResultDTO::Wrapper>
    );

public:
    PrescriptionSplitJsonVO()
    {
        data = PrescriptionSplitResultDTO::createShared();
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif
