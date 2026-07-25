#pragma once
#ifndef PATIENTQUEUEMAPPER_H_
#define PATIENTQUEUEMAPPER_H_


#include "Mapper.h"
#include "domain/do/cycleCtrl1/BkgWaitlistEntryDO.h"
#include "domain/do/cycleCtrl1/AdmHealthcareServiceDO.h"
#include "domain/do/cycleCtrl1/AdmPatientDO.h"
#include "domain/do/cycleCtrl1/BkgScheduleTemplateDO.h"


//Mapper是读取已经被select出来的列
class PatientQueueMapper : public Mapper<PtrBkgWaitlistEntryDO>
{
public:
	PtrBkgWaitlistEntryDO mapper(ResultSet* resultSet) const override
	{
		PtrBkgWaitlistEntryDO entry = std::make_shared<BkgWaitlistEntryDO>();

		entry->setId(resultSet->getString("id"));
		entry->setPatientId(resultSet->getString("patient_id"));
		entry->setScheduleId(resultSet->getString("schedule_id"));
		entry->setStatus(resultSet->getString("status"));
		entry->setCreatedAt(resultSet->getString("created_at"));

		//读取患者信息，left join可能为空
		std::string patientName = resultSet->getString("name");
		if(!patientName.empty())
		{
			auto patientDO = std::make_shared<AdmPatientDO>();
			patientDO->setName(patientName);
			patientDO->setGenderEnum(resultSet->getString("gender_enum"));
			patientDO->setBirthDate(resultSet->getString("birth_date"));
			patientDO->setBusNo(resultSet->getString("bus_no"));
			entry->setPatientDO(patientDO);
		}

		//读取挂号信息，left join可能为空
		std::string serviceName = resultSet->getString("service_name");
		if(!serviceName.empty())
		{
			auto serviceDO = std::make_shared<AdmHealthcareServiceDO>();
			serviceDO->setName(serviceName);
			entry->setHealthcareServiceDO(serviceDO);
		}

		return entry;
	}

};

#endif // !PATIENTQUEUEMAPPER_H_
