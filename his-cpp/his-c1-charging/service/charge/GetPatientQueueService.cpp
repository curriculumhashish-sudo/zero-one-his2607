#include "stdafx.h"
#include "GetPatientQueueService.h"
#include "dao/charge/GetPatientQueueDAO.h"
#include "dao/charge/ChargeItemBriefDAO.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

#include <algorithm>
#include <map>
#include <ctime>
#include <utility>

// ============================================================================
// 辅助方法：枚举值转字典文本、年龄计算、状态聚合
// ============================================================================

std::string GetPatientQueueService::translateGender(const std::string& code)
{
    if (code == "MALE")   return ZH_WORDS_GETTER("lhfc.charging.charge.dict.gender.male");
    if (code == "FEMALE") return ZH_WORDS_GETTER("lhfc.charging.charge.dict.gender.female");
    return code;
}

std::string GetPatientQueueService::computeAgeText(const std::string& birthDate)
{
    if (birthDate.empty() || birthDate.length() < 4) return "";

    int birthYear = 0;
    try { birthYear = std::stoi(birthDate.substr(0, 4)); }
    catch (...) { return ""; }

    time_t now = time(nullptr);
    struct tm* tmNow = localtime(&now);
    int currentYear = tmNow->tm_year + 1900;
    int age = currentYear - birthYear;
    if (age < 0) return "";

    return std::to_string(age) + ZH_WORDS_GETTER("lhfc.charging.charge.dict.ageSuffix");
}

std::string GetPatientQueueService::translateChargeStatus(const std::string& code)
{
    if (code == "DRAFT")     return ZH_WORDS_GETTER("lhfc.charging.charge.dict.chargeStatus.pending");
    if (code == "SETTLED")   return ZH_WORDS_GETTER("lhfc.charging.charge.dict.chargeStatus.settled");
    if (code == "REFUNDED")  return ZH_WORDS_GETTER("lhfc.charging.charge.dict.chargeStatus.refunded");
    return code;
}

std::string GetPatientQueueService::computeRollupStatusCode(
    const std::list<std::string>& statusCodes)
{
    if (statusCodes.empty()) return "PLANNED";

    bool hasPending = false;
    bool hasSettled = false;

    for (auto& code : statusCodes)
    {
        if (code == "DRAFT") hasPending = true;
        else if (code == "SETTLED") hasSettled = true;
        // REFUNDED: already refunded, exclude from rollup
    }

    if (hasPending && hasSettled) return "MIXED";
    if (hasPending) return "PLANNED";
    return "SETTLED";
}

std::string GetPatientQueueService::translateRollupStatus(const std::string& code)
{
    if (code == "PLANNED") return ZH_WORDS_GETTER("lhfc.charging.charge.dict.rollupStatus.planned");
    if (code == "SETTLED") return ZH_WORDS_GETTER("lhfc.charging.charge.dict.rollupStatus.settled");
    if (code == "MIXED")   return ZH_WORDS_GETTER("lhfc.charging.charge.dict.rollupStatus.mixed");
    return code;
}

// ============================================================================
// 主方法：分步执行查询、转换、组装
// ============================================================================

GetPatientQueuePageDTO::Wrapper GetPatientQueueService::listAll(
    const GetPatientQueueQuery::Wrapper& query)
{
    // 第1步：查询符合条件的总记录数
    GetPatientQueueDAO mainDao;
    uint64_t totalCount = mainDao.count(query);

    // 第2步：分页查询当前页的主数据
    auto doList = mainDao.selectWithPage(query);
    std::cout << "mainDo count "<< doList.size() << std::endl;
    // 第3步：收集当前页所有就诊ID，用于批量加载子数据
    std::list<std::string> encounterIds;
    for (auto& dobj : doList)
    {
        encounterIds.push_back(dobj.getEncounterId());
    }

    // 第4步：批量查询收费项摘要，按就诊ID分组
    std::map<std::string, std::list<ChargeItemBriefDO>> chargeItemMap;
    if (!encounterIds.empty())
    {
        ChargeItemBriefDAO ciDao;
        auto ciList = ciDao.selectByEncounterIds(encounterIds);
        for (auto& ci : ciList)
        {
            chargeItemMap[ci.getEncounterId()].push_back(std::move(ci));
        }
    }

    // 第5步：初始化分页DTO，填充页码、总数等元信息
    auto pageDTO = GetPatientQueuePageDTO::createShared();
    pageDTO->pageIndex = query->pageIndex.getValue(1);
    pageDTO->pageSize  = query->pageSize.getValue(10);
    pageDTO->total = static_cast<int64_t>(totalCount);
    pageDTO->calcPages();
    pageDTO->rows = oatpp::List<GetPatientQueueDTO::Wrapper>::createShared();

    if (doList.empty())
        return pageDTO;

    // 第6步：逐行把 DO 转换为 DTO，同时计算和翻译字段
    for (auto& dobj : doList)
    {
        auto dto = GetPatientQueueDTO::createShared();

        // 直接映射的字段，类型相同无需转换
        ZO_STAR_DOMAIN_DO_TO_DTO(dto, dobj,
            encounterId, EncounterId,
            encounterBusNo, EncounterBusNo,
            patientId, PatientId,
            patientName, PatientName,
            genderCode, GenderCode,
            organizationName, OrganizationName,
            doctorName, DoctorName,
            startTime, StartTime,
            paymentId, PaymentId
        );

        // 字典翻译字段
        dto->genderEnumText = translateGender(dobj.getGenderCode());

        // 计算字段
        dto->ageText = computeAgeText(dobj.getBirthDate());

        // 类型转换：string -> Float64
        try { dto->totalAmount = std::stod(dobj.getTotalAmount()); }
        catch (...) { dto->totalAmount = 0.0; }
        try { dto->paidAmount = std::stod(dobj.getPaidAmount()); }
        catch (...) { dto->paidAmount = 0.0; }

        // 组装收费项摘要子列表
        auto& ciList = chargeItemMap[dobj.getEncounterId()];
        auto itemVec = oatpp::Vector<oatpp::Object<ChargeItemBriefDTO>>::createShared();
        std::list<std::string> statusCodesForRollup;

        for (auto& ci : ciList)
        {
            auto ciDto = ChargeItemBriefDTO::createShared();
            ciDto->chargeItemId = ci.getChargeItemId();
            ciDto->statusCode = ci.getStatusCode();
            ciDto->statusText = translateChargeStatus(ci.getStatusCode());
            try { ciDto->totalPrice = std::stod(ci.getTotalPrice()); }
            catch (...) { ciDto->totalPrice = 0.0; }
            ciDto->itemLabel = ci.getItemLabel();

            itemVec->push_back(ciDto);
            statusCodesForRollup.push_back(ci.getStatusCode());
        }
        dto->cashierChargeItems = itemVec;

        // 根据收费项状态判定队列标签：有任一待收费则为 PENDING
        bool hasPendingItem = std::any_of(statusCodesForRollup.begin(),
            statusCodesForRollup.end(),
            [](const std::string& s) { return s == "DRAFT"; });
        dto->queueLabelCode = hasPendingItem ? "PENDING" : "SETTLED";

        // 聚合所有收费项状态得出汇总状态码
        dto->cashierChargeRollupStatusCode =
            computeRollupStatusCode(statusCodesForRollup);

        // 汇总状态码转展示文本
        dto->cashierChargeRollupStatusText =
            translateRollupStatus(dto->cashierChargeRollupStatusCode);

        // 加入当前页数据列表
        pageDTO->addData(dto);
    }

    return pageDTO;
}
