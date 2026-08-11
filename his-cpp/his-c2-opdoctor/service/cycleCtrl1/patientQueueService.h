#pragma once
#ifndef _PATIENTQUEUESERVICE_H_
#define _PATIENTQUEUESERVICE_H_

#include "domain/query/cycleCtrl1/PatientQueueQuery.h"
#include "domain/dto/cycleCtrl1/PatientQueueDTO.h"
#include "dao/cycleCtrl1/PatientQueueDAO.h"
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "EnumDictUtil.h"
#include <memory>
#include <string>

class PatientQueueService
{
public:
    // 返回患者队列列表
    oatpp::Vector<PatientQueueDTO::Wrapper> getPatientQueueList(const PatientQueueQuery::Wrapper& query,
        const std::string& tenantId,
        const std::string& practitionerId);

    static int calcAge(const std::string& birthDate);

    PatientQueueService()
        : m_mongoClient(make_shared<MongoClient>("mongodb://994xyQ10gK1h:2rNNkUEG8C8syEOmJBK66WUnh24g4BKS@47.113.229.85:41242/hisDb"))

    {
    }

private:
    PatientQueueDAO m_patientQueueDAO;
    std::shared_ptr<MongoClient> m_mongoClient;
};

#endif // !_PATIENTQUEUESERVICE_H_