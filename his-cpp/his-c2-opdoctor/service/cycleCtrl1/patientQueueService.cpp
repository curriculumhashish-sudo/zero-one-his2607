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

        item->age = 0;
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

        item->medicalRecordNum = encDO->getBusNo();
        item->payType = accDO->getTypeCode();
        item->visitStatus = encDO->getStatusEnum();

        resultVec->push_back(item);
    }

    return resultVec;
}