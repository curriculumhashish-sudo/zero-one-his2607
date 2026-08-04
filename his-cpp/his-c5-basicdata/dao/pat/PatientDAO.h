#pragma once
#ifndef _PATIENT_DAO_H_
#define _PATIENT_DAO_H_

#include "BaseDAO.h"
#include "../../domain/do/pat/PatientDO.h"
#include "domain/dto/pat/PatientDTO.h"
#include "domain/query/pat/PatSelectPageQuery.h"
#include "domain/query/pat/PatPageQuery.h"
#include <list>

class PatientDAO : public BaseDAO
{
public:
    std::list<PtrPatientDO> selectPatList(const PatSelectPageQuery::Wrapper& query, const std::string& tenantId);
    uint64_t countPatList(const PatSelectPageQuery::Wrapper& query, const std::string& tenantId);
    std::list<PtrPatientDO> queryPatPage(const PatPageQuery::Wrapper& query, const std::string& tenantId);
    uint64_t countPatPage(const PatPageQuery::Wrapper& query, const std::string& tenantId);

    std::string selectTenantIdByUserId(const std::string& userId);
    PtrPatientDO selectById(const std::string& id, const std::string& tenantId);
    PtrPatientDO selectByIdForUpdate(const std::string& id);
    bool existsBusNo(const std::string& tenantId, const std::string& busNo, const std::string& excludeId = "");
    bool existsIdCard(const std::string& tenantId, const std::string& idCard, const std::string& excludeId = "");
    bool organizationExists(const std::string& tenantId, const std::string& organizationId);
    int insertPatient(const PatientDO& data);
    int updatePatch(const PatientUpdateDTO& dto, const std::string& tenantId, const std::string& userId, int expectedVersion);

    int softDeletePatient(const std::string& id, const std::string& tenantId,
        const std::string& deletedAt, const std::string& deletedBy);

private:
    std::string buildSelectCondition(const PatSelectPageQuery::Wrapper& query, const std::string& tenantId, SqlParams& params);
    std::string buildPatPageCondition(const PatPageQuery::Wrapper& query, const std::string& tenantId, SqlParams& params);
};

#endif // !_PATIENT_DAO_H_
