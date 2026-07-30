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

    // 填充 DTO 字段
    dto->patientName = data->getPatientDO()->getName();
	dto->visitStatus = data->getEncounterDO()->getStatusEnum();
    dto->gender = data->getPatientDO()->getGenderEnum(); 
	dto->age = calcAge(data->getPatientDO()->getBirthDate());
    dto->registerType = data->getServiceDO()->getName();
    dto->encounterId = data->getEncounterDO()->getBusNo();
    dto->payType = data->getAccountDO()->getTypeCode();
    dto->VisitTime = data->getEncounterDO()->getReceptionTime();
    dto->registerDept = data->getOrgDO()->getName();
    dto->phoneNum = data->getPatientDO()->getPhone();
    dto->allergyHistory = data->getAllergyDO()->getAllergyText();


    return dto;
}


int ReceptionPatientService::calcAge(const std::string& birthDate)
{
    if (birthDate.empty())
        return 0;

    // 解析出生日期 yyyy-MM-dd
    std::tm birthTm = {};
    std::istringstream ss(birthDate);
    ss >> std::get_time(&birthTm, "%Y-%m-%d");
    if (ss.fail())
        return 0;

    // 当前系统时间
    std::time_t nowT = std::time(nullptr);
    std::tm nowTm = *std::localtime(&nowT);

    int age = nowTm.tm_year - birthTm.tm_year;
    // 判断生日有没有过
    if (nowTm.tm_mon < birthTm.tm_mon ||
        (nowTm.tm_mon == birthTm.tm_mon && nowTm.tm_mday < birthTm.tm_mday))
    {
        age--;
    }
    return age > 0 ? age : 0;
}