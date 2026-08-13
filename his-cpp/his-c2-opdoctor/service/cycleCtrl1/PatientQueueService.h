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
#include "service/ClientManager.h"

class PatientQueueService
{
public:
    // 返回患者队列列表
    oatpp::Vector<PatientQueueDTO::Wrapper> getPatientQueueList(const PatientQueueQuery::Wrapper& query,
        const std::string& tenantId,
        const std::string& practitionerId);

    static int calcAge(const std::string& birthDate);

    PatientQueueService() = default; // 默认构造，去掉m_mongoClient赋值

private:
    PatientQueueDAO m_patientQueueDAO;
    // 保留成员也可以，但不再构造函数赋值
    std::shared_ptr<MongoClient> m_mongoClient;
};

#endif // !_PATIENTQUEUESERVICE_H_