#pragma once
#ifndef _PATIENTQUEUESERVICE_H_
#define _PATIENTQUEUESERVICE_H_

#include "domain/query/cycleCtrl1/patientQueueQuery.h"
#include "domain/dto/cycleCtrl1/patientQueueDTO.h"
#include "dao/cycleCtrl1/PatientQueueDAO.h"

class PatientQueueService
{
public:
    //返回患者队列列表
        oatpp::Vector<PatientQueueDTO::Wrapper> getPatientQueueList(const PatientQueueQuery::Wrapper& query,
        const std::string& tenantId,
        const std::string& practitionerId);


private:
    PatientQueueDAO m_patientQueueDAO;
};

#endif // !_PATIENTQUEUESERVICE_H_