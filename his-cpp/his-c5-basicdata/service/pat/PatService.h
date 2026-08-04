#pragma once
#ifndef _PAT_SERVICE_H_
#define _PAT_SERVICE_H_

#include "ResultStatus.h"
#include "../../domain/query/pat/PatSelectPageQuery.h"
#include "../../domain/query/pat/PatPageQuery.h"
#include "../../domain/query/pat/PatientQuery.h"
#include "../../domain/dto/pat/PatSelectDTO.h"
#include "../../domain/dto/pat/PatientPageItemDTO.h"
#include "../../domain/dto/pat/PatientDTO.h"
#include "domain/dto/PageDTO.h"
#include "domain/dto/PayloadDTO.h"
#include <string>

struct PatientOperationResult
{
    PatientOperationResult(bool success, const std::string& id, const ResultStatus& status, int httpStatus)
        : success(success), id(id), status(status), httpStatus(httpStatus)
    {
    }

    bool success;
    std::string id;
    ResultStatus status;
    int httpStatus;
};

struct PatientDetailResult
{
    PatientDetailResult(bool success, const PatientDetailDTO::Wrapper& data,
        const ResultStatus& status, int httpStatus)
        : success(success), data(data), status(status), httpStatus(httpStatus)
    {
    }

    bool success;
    PatientDetailDTO::Wrapper data;
    ResultStatus status;
    int httpStatus;
};

class PatService
{
public:
    PageDTO<PatSelectDTO::Wrapper>::Wrapper querySelectPatPage(
        const PatSelectPageQuery::Wrapper& query,
        const PayloadDTO& payload);

    PageDTO<PatientPageItemDTO::Wrapper>::Wrapper queryPatPage(
        const PatPageQuery::Wrapper& query,
        const PayloadDTO& payload);

    PatientDetailResult queryById(const PatientDetailQuery::Wrapper& query);
    PatientOperationResult add(const PatientAddDTO::Wrapper& dto);
    PatientOperationResult modify(const PatientUpdateDTO::Wrapper& dto);

    bool removePatient(const std::string& id, const PayloadDTO& payload);

private:
    static std::string toGenderLabel(const std::string& genderEnum);
    static int toGenderCode(const std::string& genderEnum);
    static int calcAge(const std::string& birthDate);
};

#endif // !_PAT_SERVICE_H_
