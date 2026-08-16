#include "stdafx.h"
#include "EnrollService.h"
#include "../../dao/enroll/EnrollDAO.h"

#include <ctime>
#include <sstream>

/**
 * @brief 分页查询符合条件的住院登记列表。
 *
 * 创建分页返回对象，查询符合条件的记录总数；
 * 当没有符合条件的数据时，直接返回空分页对象。
 * 有数据时，后续会继续查询当前页 DO 列表并转换为 DTO。
 *
 * @param query 前端传入的住院登记查询条件，包含关键字、身份证号、
 *              登记状态、当前页码和每页条数。
 * @return 包含分页信息和住院登记列表的分页对象。
 */
EnrollPageDTO::Wrapper EnrollService::queryAll(const EnrollQuery::Wrapper& query)
{
    // 创建接口最终需要返回的分页对象。
    auto page = EnrollPageDTO::createShared();

    // 将前端传入的分页参数原样写入返回对象。
    page->pageIndex = query->pageIndex;
    page->pageSize = query->pageSize;

    //DAO只负责数据库访问，Service 负责组织业务查询流程。
    EnrollDAO dao;

    //先查询总数，工前段展示总记录数和总页数
    uint64_t total = dao.count(query);
    // 没有数据时无需继续执行分页列表查询。
    if (total == 0)
    {
        return page;
    }
    // 设置总记录数，并根据总数和每页条数计算总页数。
    page->total = total;
    page->calcPages();

    // 查询当前页的数据库记录；每一项是 DAO 返回的 EnrollListDO。
    std::list<EnrollListDO> enrollList = dao.selectWithPage(query);

    for (EnrollListDO& data : enrollList)
    {
        auto dto = EnrollListDTO::createShared();

        // 将 DO 中能够一一对应的字段复制到 DTO。
        ZO_STAR_DOMAIN_DO_TO_DTO
        (
            dto,
            data,
            encounterId, EncounterId,
            patientId, PatientId,
            encounterBusNo, EncounterBusNo,
            patientName, PatientName,
            statusEnum, StatusEnum,
            organizationId, OrganizationId,
            organizationName, OrganizationName,
            wardId, WardId,
            wardName, WardName,
            bedId, BedId,
            bedName, BedName,
            startTime, StartTime
        );

        // 将数据库状态代码转换为页面需要的中文状态文字。
        dto->statusEnumText = getStatusEnumText(data.getStatusEnum());
        dto->cancelable = isCancelable(data.getStatusEnum());

        // 将当前记录加入分页结果列表。
        page->addData(dto);
    }

    return page;
}

/**
 * @brief 判断住院登记是否允许普通取消。
 *
 * @param statusEnum 当前住院登记状态代码。
 * @return 可普通取消时返回 true，否则返回 false。
 *
 * @details
 * PLANNED 表示待登记，REGISTERED 表示待入科；
 * 当前接口按原型规则将这两种状态标记为可取消。
 */
bool EnrollService::isCancelable(const std::string& statusEnum)
{
    return statusEnum == "PLANNED" || statusEnum == "REGISTERED";
}

/**
 * @brief 将住院登记状态代码转换为页面展示文字。
 *
 * 根据状态代码读取 QQQQ 状态字典中的中文文案。
 * 集中处理状态转换，避免多个接口重复编写相同判断。
 *
 * @param statusEnum 数据库中的住院登记状态代码。
 * @return 对应的中文展示文字；未知状态时返回原始状态代码。
 */
std::string EnrollService::getStatusEnumText(const std::string& statusEnum)
{
    if (statusEnum == "PLANNED")
    {
        return ZH_WORDS_GETTER("QQQQ.status.planned");
    }

    if (statusEnum == "REGISTERED")
    {
        return ZH_WORDS_GETTER("QQQQ.status.registered");
    }

    if (statusEnum == "IN_PROGRESS")
    {
        return ZH_WORDS_GETTER("QQQQ.status.in_progress");
    }

    if (statusEnum == "CANCELED")
    {
        return ZH_WORDS_GETTER("QQQQ.status.canceled");
    }

    if (statusEnum == "CANCELLED")
    {
        return ZH_WORDS_GETTER("QQQQ.status.cancelled");
    }

    if (statusEnum == "COMPLETED")
    {
        return ZH_WORDS_GETTER("QQQQ.status.completed");
    }

    if (statusEnum == "DISCHARGED")
    {
        return ZH_WORDS_GETTER("QQQQ.status.discharged");
    }

    if (statusEnum == "ON_HOLD")
    {
        return ZH_WORDS_GETTER("QQQQ.status.on_hold");
    }

    return statusEnum;
}

/**
 * @brief 将性别代码转换为页面展示文字。
 *
 * @param genderEnum 数据库中的性别代码。
 * @return 对应的展示文字；未知代码时返回原始代码。
 *
 * @details
 * 数据库中保存 MALE、FEMALE 等稳定代码，
 * 中文展示文字由 QQQQ 字典统一维护。
 */
std::string EnrollService::getGenderEnumText(
    const std::string& genderEnum
)
{
    if (genderEnum == "MALE")
    {
        return ZH_WORDS_GETTER("QQQQ.gender.male");
    }

    if (genderEnum == "FEMALE")
    {
        return ZH_WORDS_GETTER("QQQQ.gender.female");
    }

    if (genderEnum == "UNKNOWN")
    {
        return ZH_WORDS_GETTER("QQQQ.gender.unknown");
    }

return genderEnum;
}

/**
 * @brief 根据出生日期计算当前年龄。
 *
 * @param birthDate 出生日期，格式为 YYYY-MM-DD。
 * @return 已满周岁的年龄；日期为空或格式无效时返回 0。
 *
 * @details
 * 年龄等于当前年份减出生年份。
 * 若今年生日尚未到达，则年龄需要减一。
 */
int EnrollService::calculateAge(const std::string& birthDate)
{
    int birthYear = 0;
    int birthMonth = 0;
    int birthDay = 0;
    char firstSeparator = '\0';
    char secondSeparator = '\0';

    std::istringstream stream(birthDate);

    if
    (
        !(stream >> birthYear >> firstSeparator
            >> birthMonth >> secondSeparator >> birthDay)
        || firstSeparator != '-'
        || secondSeparator != '-'
        || birthMonth < 1
        || birthMonth > 12
        || birthDay < 1
        || birthDay > 31
    )
    {
        return 0;
    }

    std::time_t now = std::time(nullptr);
    std::tm currentDate
    {
    };
#ifdef LINUX
    localtime_r(&now, &currentDate);
#else
    localtime_s(&currentDate, &now);
#endif
    int currentYear = currentDate.tm_year + 1900;
    int currentMonth = currentDate.tm_mon + 1;
    int currentDay = currentDate.tm_mday;

    int age = currentYear - birthYear;

    if
    (
        currentMonth < birthMonth
        ||
        (
            currentMonth == birthMonth
            && currentDay < birthDay
        )
    )
    {
        --age;
    }

    return age > 0 ? age : 0;
}

/**
 * @brief 根据住院登记主键查询住院登记详情。
 *
 * @param encounterId 住院登记主键，对应 adm_encounter.id。
 * @return 查询到时返回详情 DTO；未查询到时返回 nullptr。
 *
 * @details
 * DAO 负责查询数据库并返回详情 DO。
 * Service 将 DO 转换为 DTO，并补充状态文字、性别文字和年龄。
 */
EnrollDetailDTO::Wrapper EnrollService::queryById(const std::string& encounterId)
{
    EnrollDAO dao;

    PtrEnrollDetailDO data = dao.selectDetailById(encounterId);

    if (!data)
    {
        return nullptr;
    }

    auto dto = EnrollDetailDTO::createShared();

    ZO_STAR_DOMAIN_DO_TO_DTO_1
    (
        dto,
        data,
        encounterId, EncounterId,
        encounterBusNo, EncounterBusNo,
        patientId, PatientId,
        patientName, PatientName,
        idCard, IdCard,
        genderEnum, GenderEnum,
        birthDate, BirthDate,
        phone, Phone,
        statusEnum, StatusEnum,
        organizationId, OrganizationId,
        organizationName, OrganizationName,
        wardId, WardId,
        wardName, WardName,
        bedId, BedId,
        bedName, BedName,
        startTime, StartTime,
        conditionDefinitionId, ConditionDefinitionId,
        conditionDefinitionName, ConditionDefinitionName,
        diagnosisDescription, DiagnosisDescription,
        admitSourceCode, AdmitSourceCode,
        admitSourceCodeText, AdmitSourceCodeText,
        inWayCode, InWayCode,
        inWayCodeText, InWayCodeText,
        priorityEnum, PriorityEnum,
        outpatientDoctorName, OutpatientDoctorName,
        attendingDoctorId, AttendingDoctorId,
        attendingDoctorName, AttendingDoctorName,
        admissionCount, AdmissionCount
    );

    dto->statusEnumText = getStatusEnumText(data->getStatusEnum());
    dto->cancelable = isCancelable(data->getStatusEnum());
    dto->genderEnumText = getGenderEnumText(data->getGenderEnum());
    dto->age = calculateAge(data->getBirthDate());

    return dto;
}

