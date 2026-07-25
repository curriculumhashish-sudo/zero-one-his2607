#pragma once
#ifndef _PATIENTQUEUESERVICE_H_
#define _PATIENTQUEUESERVICE_H_

#include "domain/query/cycleCtrl1/patientQueueQuery.h"
#include "domain/dto/cycleCtrl1/patientQueueDTO.h"
#include "domain/do/cycleCtrl1/BkgWaitlistEntryDO.h"
#include "dao/cycleCtrl1/patientQueueDAO.h"

class PatientQueueService
{
public:
	oatpp::Vector<PatientQueueDTO::Wrapper> getPatientQueueList(const PatientQueueQuery::Wrapper& query);

private:
	inline int calcAge(const std::string& birthDate);
	PatientQueueDAO dao;

};




#endif // !_PATIENTQUEUESERVICE_H_
