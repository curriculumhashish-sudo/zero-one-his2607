#pragma once
#ifndef _DEPT_SERVICE_H_
#define _DEPT_SERVICE_H_

#include "ResultStatus.h"
#include "domain/dto/dept/DeptBatchStatusDTO.h"
#include "domain/dto/dept/deptDTO.h"
#include "domain/query/dept/DeptQuery.h"

struct DeptOperationResult
{
    DeptOperationResult(bool success, const std::string& id, const ResultStatus& status, int httpStatus)
        : success(success), id(id), status(status), httpStatus(httpStatus)
    {
    }

    bool success;
    std::string id;
    ResultStatus status;
    int httpStatus;
};

class DeptService
{
public:
    DeptOperationResult add(const DeptAddDTO::Wrapper& dto);
    DeptOperationResult modify(const DeptModifyDTO::Wrapper& dto);
    DeptOperationResult remove(const DeptDeleteQuery::Wrapper& query);
    DeptOperationResult batchStatus(const DeptBatchStatusDTO::Wrapper& dto);
};

#endif // !_DEPT_SERVICE_H_