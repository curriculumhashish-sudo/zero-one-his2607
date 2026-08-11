#include "stdafx.h"
#include "ReceptionPatientService.h"
#include "EnumDictUtil.h"
#include <mongocxx/client.hpp>
#include <sstream>

RecePatientDTO::Wrapper ReceptionPatientService::getRecePatient(const RecePatientQuery::Wrapper& query,
    const string& tenantId, const string& practitionerId)
{
    auto data = m_dao.queryReceptionPatient(query, tenantId, practitionerId);
    auto dto = RecePatientDTO::createShared();

    static const std::unordered_map<std::string, std::string> payTypeMapping = {
         {"SELF_PAY", u8"自费"},
         {"MEDICAL_INSURANCE",u8"医保"}
    };

    if (data == nullptr)
    {
        return dto;
    }

    // 加载状态枚举字典
    std::unordered_map<std::string, std::string> statusDict;
    if (m_mongoClient)
    {
        statusDict = EnumDictUtil::loadEnumDict(*m_mongoClient, tenantId, "visit_status");
    }

    // 填充 DTO 字段
    dto->patientName = data->getPatientDO()->getName();

    std::string statusCode = data->getEncounterDO()->getStatusEnum();
    std::string statusText = "未知";
    auto itDict = statusDict.find(statusCode);
    if (itDict != statusDict.end())
    {
        statusText = itDict->second;
    }
    else
    {
        static const std::unordered_map<std::string, std::string> statusMapping = {
            {"arrived", u8"待诊"},
            {"in-progress", u8"在诊"},
            {"on-hold", u8"暂离"},
            {"finished", u8"完诊"}
        };
        auto itMap = statusMapping.find(statusCode);
        if (itMap != statusMapping.end())
        {
            statusText = itMap->second;
        }
    }
    dto->visitStatus = statusText;


    // 修复：MAIL → MALE
    if (data->getPatientDO()->getGenderEnum() == "MALE")
    {
        dto->gender = u8"男性";
    }
    else if (data->getPatientDO()->getGenderEnum() == "FEMALE")
    {
        dto->gender = u8"女性";
    }
    else
    {
        dto->gender = u8"未知";
    }


    dto->age = calcAge(data->getPatientDO()->getBirthDate());
    dto->registerType = data->getServiceDO()->getName();
    dto->encounterId = data->getEncounterDO()->getId();
    dto->payType = data->getAccountDO()->getTypeCode();
    auto itPayType = payTypeMapping.find(dto->payType);
    if (itPayType != payTypeMapping.end())
    {
        dto->payType = itPayType->second;
    }
    else
    {
        dto->payType = u8"未知";
    }


    // reception_time 现在是string，直接赋值
    dto->visitTime = data->getEncounterDO()->getReceptionTime();

    // left join字段判空，防止空指针崩溃
    if (data->getOrgDO())
    {
        dto->registerDept = data->getOrgDO()->getName();
    }
    else
    {
        dto->registerDept = "";
    }

    dto->phoneNum = data->getPatientDO()->getPhone();

    if (data->getAllergyDO())
    {
        dto->allergyHistory = data->getAllergyDO()->getAllergyText();
    }
    else
    {
        dto->allergyHistory = "";
    }

    return dto;
}


int ReceptionPatientService::calcAge(const std::string& birthDate)
{
    if (birthDate.empty())
        return 0;

    // 解析出生日期 yyyy?MM?dd
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

// 追加实现：接诊动作
RecePatientDTO::Wrapper ReceptionPatientService::acceptPatientByEncounterId(
    const std::string& encounterId, const std::string& tenantId, const std::string& practitionerId)
{
    RecePatientDTO::Wrapper dto = RecePatientDTO::createShared();

    if (encounterId.empty() || tenantId.empty())
    {
        return dto;
    }

    // 先查询当前就诊记录
    auto data = m_dao.queryReceptionByEncounterId(encounterId, tenantId);
    if (!data)
        return dto;

    std::string currentStatus = data->getEncounterDO()->getStatusEnum();

    // 幂等：如果当前是 arrived，则尝试更新为 in-progress；如果已经 in-progress 或 finished，保持不变
    if (currentStatus == "arrived")
    {
        bool updated = m_dao.updateEncounterStatusIfMatch(encounterId, tenantId, "in-progress", "arrived");
        if (updated)
        {
            // 重新加载最新数据
            data = m_dao.queryReceptionByEncounterId(encounterId, tenantId);
            if (!data)
                return dto;
        }
    }

    // 无论是否更新，返回完整的患者上下文（与 getRecePatient 的填充类似）
    // 加载状态字典（尝试）
    std::unordered_map<std::string, std::string> statusDict;
    if (m_mongoClient)
    {
        statusDict = EnumDictUtil::loadEnumDict(*m_mongoClient, tenantId, "visit_status");
    }

    static const std::unordered_map<std::string, std::string> payTypeMapping = {
         {"SELF_PAY", u8"自费"},
         {"MEDICAL_INSURANCE",u8"医保"}
    };

    // 填充 DTO
    dto->patientName = data->getPatientDO()->getName();

    std::string statusCode = data->getEncounterDO()->getStatusEnum();
    std::string statusText = "未知";
    auto itDict = statusDict.find(statusCode);
    if (itDict != statusDict.end())
    {
        statusText = itDict->second;
    }
    else
    {
        static const std::unordered_map<std::string, std::string> statusMapping = {
            {"arrived", u8"待诊"},
            {"in-progress", u8"在诊"},
            {"on-hold", u8"暂离"},
            {"finished", u8"完诊"}
        };
        auto itMap = statusMapping.find(statusCode);
        if (itMap != statusMapping.end())
            statusText = itMap->second;
    }
    dto->visitStatus = statusText;

    // 性别
    if (data->getPatientDO()->getGenderEnum() == "MALE")
        dto->gender = u8"男性";
    else if (data->getPatientDO()->getGenderEnum() == "FEMALE")
        dto->gender = u8"女性";
    else
        dto->gender = u8"未知";

    dto->age = calcAge(data->getPatientDO()->getBirthDate());

    if (data->getServiceDO())
        dto->registerType = data->getServiceDO()->getName();
    else
        dto->registerType = "";

    dto->encounterId = data->getEncounterDO()->getId();

    if (data->getAccountDO())
    {
        dto->payType = data->getAccountDO()->getTypeCode();
        auto itPayType = payTypeMapping.find(dto->payType);
        if (itPayType != payTypeMapping.end())
            dto->payType = itPayType->second;
        else
            dto->payType = u8"未知";
    }
    else
    {
        dto->payType = u8"未知";
    }

    dto->visitTime = data->getEncounterDO()->getReceptionTime();

    if (data->getOrgDO())
        dto->registerDept = data->getOrgDO()->getName();
    else
        dto->registerDept = "";

    dto->phoneNum = data->getPatientDO()->getPhone();

    if (data->getAllergyDO())
        dto->allergyHistory = data->getAllergyDO()->getAllergyText();
    else
        dto->allergyHistory = "";

    return dto;
}