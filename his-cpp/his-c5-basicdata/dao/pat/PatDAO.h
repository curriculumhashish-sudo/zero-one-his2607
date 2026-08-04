#pragma once
#include "BaseDAO.h"
#include "../../domain/do/pat/PatientDO.h"
#include <list>
#include <string>

/**
 * Patient 数据操作层
 */
    class PatDAO : public BaseDAO
{
public:
    int deleteByIds(const std::list<std::string>& ids)
    {
        return BaseDAO::deleteByIds<PatientDO>(ids);
    }
};