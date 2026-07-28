#pragma once
#ifndef _DEPT_DAO_H_
#define _DEPT_DAO_H_

#include "BaseDAO.h"
#include "domain/do/dept/DeptDO.h"
#include "domain/dto/dept/deptDTO.h"

class DeptDAO : public BaseDAO
{
public:
    std::string selectTenantIdByUserId(const std::string& userId);
    PtrDeptDO selectByIdForUpdate(const std::string& id);
    bool existsName(const std::string& tenantId, const std::string& name, const std::string& excludeId = "");
    bool existsDoctor(const std::string& tenantId, const std::string& doctorId);
    bool hasChildren(const std::string& id);
    bool hasBusinessReferences(const std::string& id);
    int insertDept(const DeptDO& data);
    int updatePatch(const DeptModifyDTO& dto, const std::string& userId, bool clearOrganizationFields);
    int logicalDelete(const std::string& id, const std::string& tenantId, const std::string& userId);
    int updateBatchStatus(const std::vector<std::string>& ids, const std::string& tenantId, const std::string& userId, const std::string& statusEnum);
};

#endif // !_DEPT_DAO_H_
