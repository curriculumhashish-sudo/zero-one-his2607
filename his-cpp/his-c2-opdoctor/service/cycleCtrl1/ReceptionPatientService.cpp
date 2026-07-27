#include "stdafx.h"
#include "ReceptionPatientService.h"

RecePatientDTO::Wrapper ReceptionPatientService::getRecePatient(const RecePatientQuery::Wrapper& query,
    const string& tenantId, const string& practitionerId)
{
    auto data = m_dao.queryReceptionPatient(query, tenantId, practitionerId);
    auto dto = RecePatientDTO::createShared();

    if (data == nullptr)
    {
        return dto;
    }

    dto->patientName = data->getPatientDO()->getName();
    dto->gender = data->getPatientDO()->getGenderEnum();
    dto->age = 0;
    dto->registerType = data->getServiceDO()->getName();
    dto->medicalRecordNum = data->getEncounterDO()->getBusNo();
    dto->recordType = data->getAccountDO()->getTypeCode();
    dto->time = data->getEncounterDO()->getReceptionTime();
    dto->registerDept = data->getOrgDO()->getName();
    dto->phoneNum = data->getPatientDO()->getPhone();
    dto->allergyHistory = data->getAllergyDO()->getAllergyText();

    return dto;
}