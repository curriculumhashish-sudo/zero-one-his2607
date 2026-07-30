#include "PatientQueueService.h"
#include "dao/cycleCtrl1/PatientQueueDAO.h"
#include <ctime>

oatpp::Vector<PatientQueueDTO::Wrapper> PatientQueueService::getPatientQueueList(
    const PatientQueueQuery::Wrapper& query,
    const std::string& tenantId,
    const std::string& practitionerId)
{
    std::list<PtrPatientQueueCombineDO> dataList = m_patientQueueDAO.queryPatientQueue(query,tenantId, practitionerId);

    auto resultVec = oatpp::Vector<PatientQueueDTO::Wrapper>::createShared();

    for (auto& combineDO : dataList)
    {
        auto item = PatientQueueDTO::createShared();

        auto encDO = combineDO->getEncounterDO();
        auto patDO = combineDO->getPatientDO();
        auto servDO = combineDO->getServiceDO();
        auto accDO = combineDO->getAccountDO();

        item->patientName = patDO->getName();

        if (patDO->getGenderEnum() == "M")
            item->gender = "男性";
        else if (patDO->getGenderEnum() == "F")
            item->gender = "女性";
        else
            item->gender = "未知";

        item->age = calcAge(patDO->getBirthDate());
        item->registerType = servDO->getName();

        int64_t ts = encDO->getReceptionTime();
        if (ts > 0)
        {
            time_t t = static_cast<time_t>(ts);
            tm* lt = localtime(&t);
            char buf[64] = { 0 };
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", lt);
            item->visitTime = buf;
        }
        else
        {
            item->visitTime = "";
        }

        item->encounterId = encDO->getBusNo();
        item->payType = accDO->getTypeCode();
        item->visitStatus = encDO->getStatusEnum();

        resultVec->push_back(item);
    }

    return resultVec;
}

int PatientQueueService::calcAge(const std::string& birthDate)
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
