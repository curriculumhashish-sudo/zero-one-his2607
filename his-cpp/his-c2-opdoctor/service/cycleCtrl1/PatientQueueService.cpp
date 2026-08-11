#include "PatientQueueService.h"
#include "dao/cycleCtrl1/PatientQueueDAO.h"
#include <ctime>
#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <sstream>

oatpp::Vector<PatientQueueDTO::Wrapper> PatientQueueService::getPatientQueueList(
    const PatientQueueQuery::Wrapper& query,
    const std::string& tenantId,
    const std::string& practitionerId)
{
    std::unordered_map<std::string, std::string> statusDict;
    if (m_mongoClient)
    {
        statusDict = EnumDictUtil::loadEnumDict(*m_mongoClient, tenantId, "visit_status");
    }

    std::list<PtrPatientQueueCombineDO> dataList = m_patientQueueDAO.queryPatientQueue(query, tenantId, practitionerId);
    auto resultVec = oatpp::Vector<PatientQueueDTO::Wrapper>::createShared();

    static const std::unordered_map<std::string, std::string> statusMapping = {
        {"arrived", u8"待诊"},
        {"in-progress", u8"在诊"},
        {"on-hold", u8"暂离"},
        {"finished", u8"完诊"}
    };

    static const std::unordered_map<std::string, std::string> payTypeMapping = {
        {"SELF_PAY", u8"自费"},
        {"MEDICAL_INSURANCE",u8"医保"}
    };

    for (auto& combineDO : dataList)
    {
        auto item = PatientQueueDTO::createShared();

        auto encDO = combineDO->getEncounterDO();
        auto patDO = combineDO->getPatientDO();
        auto servDO = combineDO->getServiceDO();
        auto accDO = combineDO->getAccountDO();

        item->patientName = patDO->getName();

        if (patDO->getGenderEnum() == "MALE")
        {
            item->gender = u8"男性";
        }
        else if (patDO->getGenderEnum() == "FEMALE")
        {
            item->gender = u8"女性";
        }
        else
        {
            item->gender = u8"未知";
        }


        item->age = calcAge(patDO->getBirthDate());
        item->registerType = servDO->getName();

        // ========= 改动：receptionTime现在是std::string，不再是int64时间戳 =========
        std::string recTimeStr = encDO->getReceptionTime();
        if (!recTimeStr.empty())
        {
            // 数据库格式 "2026-07-29 09:10:00"，截取到分 "2026-07-29 09:10"
            if (recTimeStr.size() >= 16)
            {
                item->visitTime = recTimeStr.substr(0, 16);
            }
            else
            {
                item->visitTime = recTimeStr;
            }
        }
        else
        {
            item->visitTime = "";
        }

        item->encounterId = encDO->getId();
        item->payType = accDO->getTypeCode();

        auto itPayType = payTypeMapping.find(item->payType);
        if (itPayType != payTypeMapping.end())
        {
            item->payType = itPayType->second;
        }
        else
        {
            item->payType = u8"未知";
        }

        std::string statusCode = encDO->getStatusEnum();
        std::string visitStatusText = "未知";

        // 直接用mysql取出的statusCode匹配Mongo enumKey
        auto itDict = statusDict.find(statusCode);
        if (itDict != statusDict.end())
        {
            visitStatusText = itDict->second;
        }
        else
        {
            // 降级本地映射
            auto itMap = statusMapping.find(statusCode);
            if (itMap != statusMapping.end())
            {
                visitStatusText = itMap->second;
            }
        }
        item->visitStatus = visitStatusText;

        resultVec->push_back(item);
    }

    return resultVec;
}

int PatientQueueService::calcAge(const std::string& birthDate)
{
    if (birthDate.empty())
        return 0;

    std::tm birthTm = {};
    std::istringstream ss(birthDate);
    ss >> std::get_time(&birthTm, "%Y-%m-%d");
    if (ss.fail())
        return 0;

    std::time_t nowT = std::time(nullptr);
    std::tm nowTm = *std::localtime(&nowT);

    int age = nowTm.tm_year - birthTm.tm_year;
    if (nowTm.tm_mon < birthTm.tm_mon ||
        (nowTm.tm_mon == birthTm.tm_mon && nowTm.tm_mday < birthTm.tm_mday))
    {
        age--;
    }
    return age > 0 ? age : 0;
}