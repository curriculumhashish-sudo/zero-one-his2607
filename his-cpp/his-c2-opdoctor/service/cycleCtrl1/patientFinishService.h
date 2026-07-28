#pragma once
#ifndef _PATIENT_FINISH_SERVICE_
#define _PATIENT_FINISH_SERVICE_


#include "dao/cycleCtrl1/patientFinishDAO.h"
#include "domain/dto/cycleCtrl1/patientFinishDTO.h"
#include "domain/vo/cycleCtrl1/patientFinishVO.h"

class PatientFinishService
{
public:
    // 完诊业务入口
    PatientFinishJsonVO::Wrapper execFinishPatient(const oatpp::String& tenantId, const oatpp::String& encounterId);

private:
    PatientFinishDAO dao;
};

#endif // !_PATIENT_FINISH_SERVICE_