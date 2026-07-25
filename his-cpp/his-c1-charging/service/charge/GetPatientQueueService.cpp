#include "stdafx.h"
#include "GetPatientQueueService.h"
#include "../../dao/charge/GetPatientQueueDAO.h"
#include "../../dao/charge/ChargeItemBriefDAO.h"

// 防止 ODBC/MySQL 类型冲突
#include "NacosClient.h"

// ============================================================================
// 私有辅助方法：枚举翻译与计算
// ============================================================================

std::string GetPatientQueueService::translateGender(const std::string& code)
{
    if (code == "MALE")   return "男";
    if (code == "FEMALE") return "女";
    return code;  // 兜底：返回编码原文
}

std::string GetPatientQueueService::computeAgeText(const std::string& birthDate)
{
    if (birthDate.empty() || birthDate.length() < 4) return "";

    int birthYear = 0;
    try { birthYear = std::stoi(birthDate.substr(0, 4)); }
    catch (...) { return ""; }

    // 获取当前年份
    time_t now = time(nullptr);
    struct tm* tmNow = localtime(&now);
    int currentYear = tmNow->tm_year + 1900;

    int age = currentYear - birthYear;
    if (age < 0) return "";

    return std::to_string(age) + "岁";
}

std::string GetPatientQueueService::translateChargeStatus(const std::string& code)
{
    if (code == "PENDING")   return "待收费";
    if (code == "SETTLED")   return "已结算";
    if (code == "REFUNDING") return "退费中";
    if (code == "REFUNDED")  return "已退费";
    return code;  // 兜底
}

std::string GetPatientQueueService::computeRollupStatusCode(
    const std::list<std::string>& statusCodes)
{
    if (statusCodes.empty()) return "planned";

    bool hasPending = false;
    bool hasSettled = false;

    for (auto& code : statusCodes)
    {
        if (code == "PENDING") hasPending = true;
        else hasSettled = true;  // 非 PENDING 状态归入已结算侧
    }

    if (hasPending && hasSettled) return "mixed";
    if (hasPending) return "planned";
    return "settled";
}

std::string GetPatientQueueService::translateRollupStatus(const std::string& code)
{
    if (code == "planned") return "计划中";
    if (code == "settled") return "已结算";
    if (code == "mixed")   return "混合";
    return code;
}

// ============================================================================
// 主方法：分页查询编排
// ============================================================================

GetPatientQueuePageDTO::Wrapper GetPatientQueueService::listAll(
    const GetPatientQueueQuery::Wrapper& query)
{
    // ---------- 第1步：查总数 ----------
    GetPatientQueueDAO mainDao;
    uint64_t totalCount = mainDao.count(query);

    // ---------- 第2步：分页查主数据 ----------
    auto doList = mainDao.selectWithPage(query);

    // ---------- 第3步：收集就诊ID，供批量子查询 ----------
    std::list<std::string> encounterIds;
    for (auto& dobj : doList)
    {
        encounterIds.push_back(dobj.getEncounterId());
    }

    // ---------- 第4步：批量查收费项摘要 ----------
    std::map<std::string, std::list<ChargeItemBriefDO>> chargeItemMap;
    if (!encounterIds.empty())
    {
        ChargeItemBriefDAO ciDao;
        auto ciList = ciDao.selectByEncounterIds(encounterIds);
        for (auto& ci : ciList)
        {
            chargeItemMap[ci.getEncounterId()].push_back(ci);
        }
    }

    // ---------- 第5步：组装分页DTO ----------
    auto pageDTO = GetPatientQueuePageDTO::createShared();

    uint64_t pageIndex = query->pageIndex ? query->pageIndex.getValue(1) : 1;
    uint64_t pageSize  = query->pageSize  ? query->pageSize.getValue(10) : 10;
    int64_t total = static_cast<int64_t>(totalCount);
    int64_t pages = (totalCount == 0) ? 0
        : static_cast<int64_t>((totalCount + pageSize - 1) / pageSize);

    pageDTO->initAll(
        pageIndex,
        pageSize,
        total,
        pages,
        oatpp::List<GetPatientQueueDTO::Wrapper>::createShared()
    );

    if (doList.empty())
        return pageDTO;

    // ---------- 第6步：逐行 DO → DTO 转换 ----------
    for (auto& dobj : doList)
    {
        auto dto = GetPatientQueueDTO::createShared();

        // --- 直塞字段（string → String，使用 ZO_STAR_DOMAIN_DO_TO_DTO 宏批量映射） ---
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

        // --- 翻译字段 ---
        dto->genderEnumText = translateGender(dobj.getGenderCode());

        // --- 计算字段 ---
        dto->ageText = computeAgeText(dobj.getBirthDate());

        // --- 类型转换字段（string → Float64） ---
        try { dto->totalAmount = std::stod(dobj.getTotalAmount()); }
        catch (...) { dto->totalAmount = 0.0; }
        try { dto->paidAmount = std::stod(dobj.getPaidAmount()); }
        catch (...) { dto->paidAmount = 0.0; }

        // --- 子列表：收费项摘要 ---
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

        // --- 内存计算：队列标签 ---
        bool hasPendingItem = std::any_of(statusCodesForRollup.begin(),
            statusCodesForRollup.end(),
            [](const std::string& s) { return s == "PENDING"; });
        dto->queueLabelCode = hasPendingItem ? "PENDING" : "SETTLED";

        // --- 内存计算：收费汇总状态 ---
        dto->cashierChargeRollupStatusCode =
            computeRollupStatusCode(statusCodesForRollup);

        // --- 翻译：汇总状态文案 ---
        dto->cashierChargeRollupStatusText =
            translateRollupStatus(dto->cashierChargeRollupStatusCode);

        // --- 添加到分页列表 ---
        pageDTO->addData(dto);
    }

    return pageDTO;
}
