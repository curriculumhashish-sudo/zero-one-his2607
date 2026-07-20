#pragma once

#ifndef _PRESCRIPTION_OPERATION_RESULT_DTO_H_
#define _PRESCRIPTION_OPERATION_RESULT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 删除医嘱响应数据
 */
class PrescriptionDeleteResultDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionDeleteResultDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD(Int32, affectedCount);
    DTO_FIELD(String, message);

public:
    PrescriptionDeleteResultDTO()
    {
        success = true;
        affectedCount = 2;
        message = "医嘱删除成功";
    }
};

/**
 * 并组医嘱响应数据
 */
class PrescriptionMergeResultDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionMergeResultDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD(Int32, affectedCount);
    DTO_FIELD(String, groupNo);
    DTO_FIELD(String, message);

public:
    PrescriptionMergeResultDTO()
    {
        success = true;
        affectedCount = 3;
        groupNo = "GROUP-20260720-001";
        message = "医嘱并组成功";
    }
};

/**
 * 拆组医嘱响应数据
 */
class PrescriptionSplitResultDTO : public oatpp::DTO
{
    DTO_INIT(PrescriptionSplitResultDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD(Int32, affectedCount);
    DTO_FIELD(String, message);

public:
    PrescriptionSplitResultDTO()
    {
        success = true;
        affectedCount = 2;
        message = "医嘱拆组成功";
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif