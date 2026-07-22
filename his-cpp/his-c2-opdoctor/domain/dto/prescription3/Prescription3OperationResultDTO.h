#pragma once

#ifndef _PRESCRIPTION3_OPERATION_RESULT_DTO_H_
#define _PRESCRIPTION3_OPERATION_RESULT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 删除医嘱响应数据
 */
class Prescription3DeleteResultDTO : public oatpp::DTO
{
    DTO_INIT(Prescription3DeleteResultDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD(Int32, affectedCount);

public:
    Prescription3DeleteResultDTO()
    {
        success = true;
        affectedCount = 2;
    }
};

/**
 * 并组医嘱响应数据
 */
class Prescription3MergeResultDTO : public oatpp::DTO
{
    DTO_INIT(Prescription3MergeResultDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD(Int32, affectedCount);
    DTO_FIELD(String, groupNo);

public:
    Prescription3MergeResultDTO()
    {
        success = true;
        affectedCount = 3;
        groupNo = "GROUP-20260721-001";
    }
};

/**
 * 拆组医嘱响应数据
 */
class Prescription3SplitResultDTO : public oatpp::DTO
{
    DTO_INIT(Prescription3SplitResultDTO, DTO);

    DTO_FIELD(Boolean, success);
    DTO_FIELD(Int32, affectedCount);

public:
    Prescription3SplitResultDTO()
    {
        success = true;
        affectedCount = 2;
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif