#pragma once
#ifndef _DEPT_MAPPER_H_
#define _DEPT_MAPPER_H_

#include "Mapper.h"
#include "domain/do/dept/DeptDO.h"

class DeptMapper : public Mapper<PtrDeptDO>
{
public:
    PtrDeptDO mapper(ResultSet* resultSet) const override;
};

class DeptStringMapper : public Mapper<std::string>
{
public:
    explicit DeptStringMapper(const std::string& column) : column(column) {}
    std::string mapper(ResultSet* resultSet) const override;

private:
    std::string column;
};

#endif // !_DEPT_MAPPER_H_
