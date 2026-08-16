#include "stdafx.h"
#include "PatService.h"

#include "TransactionManager.h"
#include "../../dao/pat/PatientDAO.h"
#include "domain/pat/PatientStatus.h"
#include "SimpleDateTimeFormat.h"
#include "id/SnowFlake.h"
#include <ctime>
#include <functional>

namespace
{
    PatientOperationResult operationSuccess(const std::string& id)
    {
        return PatientOperationResult(true, id, RS_SUCCESS, 200);
    }

    PatientOperationResult operationFailure(const ResultStatus& status, int httpStatus)
    {
        return PatientOperationResult(false, "", status, httpStatus);
    }

    PatientDetailResult detailFailure(const ResultStatus& status, int httpStatus)
    {
        return PatientDetailResult(false, nullptr, status, httpStatus);
    }

    std::string valueOrEmpty(const oatpp::String& value)
    {
        return value ? value->c_str() : "";
    }

    void setOptionalString(const oatpp::String& value, const std::function<void(const std::string&)>& setter)
    {
        if (value) setter(value->c_str());
    }

    PatientDetailDTO::Wrapper toDetailDTO(const PtrPatientDO& data)
    {
        auto dto = PatientDetailDTO::createShared();
        dto->id = data->getId();
        dto->version = data->getVersion();
        dto->busNo = data->getBusNo();
        dto->name = data->getName();
        dto->genderEnum = data->getGender();
        dto->statusEnum = data->getStatusEnum();
        dto->activeFlag = data->getActiveFlag() != 0;
        dto->tempFlag = data->getTempFlag() != 0;
        dto->organizationId = data->getOrganizationId();

        if (data->getNameJsonPtr()) dto->nameJson = data->getNameJson();
        if (data->getBirthDatePtr()) dto->birthDate = data->getBirthDate();
        if (data->getIdCardPtr()) dto->idCard = data->getIdCard();
        if (data->getMaritalStatusEnumPtr()) dto->maritalStatusEnum = data->getMaritalStatusEnum();
        if (data->getPrfsEnumPtr()) dto->prfsEnum = data->getPrfsEnum();
        if (data->getPhonePtr()) dto->phone = data->getPhone();
        if (data->getAddressPtr()) dto->address = data->getAddress();
        if (data->getAddressProvincePtr()) dto->addressProvince = data->getAddressProvince();
        if (data->getAddressCityPtr()) dto->addressCity = data->getAddressCity();
        if (data->getAddressDistrictPtr()) dto->addressDistrict = data->getAddressDistrict();
        if (data->getAddressStreetPtr()) dto->addressStreet = data->getAddressStreet();
        if (data->getAddressExtendJsonPtr()) dto->addressExtendJson = data->getAddressExtendJson();
        if (data->getNationalityCodePtr()) dto->nationalityCode = data->getNationalityCode();
        if (data->getPyStrPtr()) dto->pyStr = data->getPyStr();
        if (data->getWbStrPtr()) dto->wbStr = data->getWbStr();
        if (data->getBloodTypeAboPtr()) dto->bloodTypeAbo = data->getBloodTypeAbo();
        if (data->getBloodTypeRhPtr()) dto->bloodTypeRh = data->getBloodTypeRh();
        if (data->getWorkCompanyPtr()) dto->workCompany = data->getWorkCompany();
        if (data->getNativePlacePtr()) dto->nativePlace = data->getNativePlace();
        if (data->getCountryCodePtr()) dto->countryCode = data->getCountryCode();
        if (data->getLinkNamePtr()) dto->linkName = data->getLinkName();
        if (data->getLinkRelationCodePtr()) dto->linkRelationCode = data->getLinkRelationCode();
        if (data->getLinkTelcomPtr()) dto->linkTelcom = data->getLinkTelcom();
        if (data->getLinkJsonsPtr()) dto->linkJsons = data->getLinkJsons();
        if (data->getDeceasedDatePtr()) dto->deceasedDate = data->getDeceasedDate();
        if (data->getDeceasedReasonPtr()) dto->deceasedReason = data->getDeceasedReason();
        if (data->getMergedTargetPatientIdPtr()) dto->mergedTargetPatientId = data->getMergedTargetPatientId();
        if (data->getMergedAtPtr()) dto->mergedAt = data->getMergedAt();
        return dto;
    }
}

std::string PatService::toGenderLabel(const std::string& genderEnum)
{
    if (genderEnum == "MALE" || genderEnum == "1") return ZH_WORDS_GETTER("pat.select.gender.male");
    if (genderEnum == "FEMALE" || genderEnum == "2") return ZH_WORDS_GETTER("pat.select.gender.female");
    return genderEnum;
}

int PatService::toGenderCode(const std::string& genderEnum)
{
    if (genderEnum == "MALE" || genderEnum == "1") return 1;
    if (genderEnum == "FEMALE" || genderEnum == "2") return 2;
    return 0;
}

int PatService::calcAge(const std::string& birthDate)
{
    if (birthDate.size() < 10) return 0;
    int year = 0;
    int month = 0;
    int day = 0;
    try
    {
        year = std::stoi(birthDate.substr(0, 4));
        month = std::stoi(birthDate.substr(5, 2));
        day = std::stoi(birthDate.substr(8, 2));
    }
    catch (...)
    {
        return 0;
    }
    if (year <= 0 || month < 1 || month > 12 || day < 1 || day > 31) return 0;

    std::time_t now = std::time(nullptr);
    std::tm localTm{};
#ifdef _WIN32
    localtime_s(&localTm, &now);
#else
    localtime_r(&now, &localTm);
#endif
    int age = localTm.tm_year + 1900 - year;
    if (localTm.tm_mon + 1 < month ||
        (localTm.tm_mon + 1 == month && localTm.tm_mday < day))
    {
        --age;
    }
    return age < 0 ? 0 : age;
}

PageDTO<PatSelectDTO::Wrapper>::Wrapper PatService::querySelectPatPage(
    const PatSelectPageQuery::Wrapper& query,
    const PayloadDTO& payload)
{
    auto pageData = PageDTO<PatSelectDTO::Wrapper>::createShared();
    pageData->pageIndex = query->pageIndex.getValue(1);
    pageData->pageSize = query->pageSize.getValue(20);
    pageData->rows = oatpp::List<PatSelectDTO::Wrapper>::createShared();

    PatientDAO dao;
    const std::string tenantId = dao.selectTenantIdByUserId(payload.getId());
    if (tenantId.empty())
    {
        pageData->total = v_int64(0);
        pageData->calcPages();
        return pageData;
    }

    const uint64_t totalCount = dao.countPatList(query, tenantId);
    pageData->total = static_cast<int64_t>(totalCount);
    pageData->calcPages();
    if (totalCount == 0) return pageData;

    for (const auto& patient : dao.selectPatList(query, tenantId))
    {
        auto dto = PatSelectDTO::createShared();
        dto->id = patient->getId();
        dto->busNo = patient->getBusNo();
        dto->name = patient->getName();
        dto->gender = toGenderLabel(patient->getGender());
        if (patient->getBirthDatePtr())
        {
            dto->birthDate = patient->getBirthDate();
            dto->age = calcAge(patient->getBirthDate());
        }
        else
        {
            dto->age = 0;
        }
        if (patient->getPhonePtr()) dto->phone = patient->getPhone();
        if (patient->getIdCardPtr()) dto->idCard = patient->getIdCard();
        pageData->addData(dto);
    }
    return pageData;
}

PageDTO<PatientPageItemDTO::Wrapper>::Wrapper PatService::queryPatPage(
    const PatPageQuery::Wrapper& query,
    const PayloadDTO& payload)
{
    auto pageData = PageDTO<PatientPageItemDTO::Wrapper>::createShared();
    pageData->pageIndex = query->pageIndex.getValue(1);
    pageData->pageSize = query->pageSize.getValue(10);
    pageData->rows = oatpp::List<PatientPageItemDTO::Wrapper>::createShared();

    PatientDAO dao;
    const std::string tenantId = dao.selectTenantIdByUserId(payload.getId());
    if (tenantId.empty())
    {
        pageData->total = v_int64(0);
        pageData->calcPages();
        return pageData;
    }

    const uint64_t totalCount = dao.countPatPage(query, tenantId);
    pageData->total = static_cast<int64_t>(totalCount);
    pageData->calcPages();
    if (totalCount == 0) return pageData;

    for (const auto& patient : dao.queryPatPage(query, tenantId))
    {
        auto dto = PatientPageItemDTO::createShared();
        dto->id = patient->getId();
        dto->busNo = patient->getBusNo();
        dto->name = patient->getName();
        dto->gender = toGenderCode(patient->getGender());
        if (patient->getBirthDatePtr()) dto->age = calcAge(patient->getBirthDate());
        else dto->age = 0;
        if (patient->getPhonePtr()) dto->phone = patient->getPhone();
        if (patient->getIdCardPtr()) dto->idCard = patient->getIdCard();
        pageData->addData(dto);
    }
    return pageData;
}

PatientDetailResult PatService::queryById(const PatientDetailQuery::Wrapper& query)
{
    try
    {
        const PayloadDTO* payload = query->getPayload();
        if (!payload) return detailFailure(PatientStatus::forbidden(), 403);

        PatientDAO dao;
        const std::string tenantId = dao.selectTenantIdByUserId(payload->getId());
        if (tenantId.empty()) return detailFailure(PatientStatus::forbidden(), 403);

        auto patient = dao.selectById(query->id.getValue(""), tenantId);
        if (!patient) return detailFailure(PatientStatus::notFound(), 404);
        return PatientDetailResult(true, toDetailDTO(patient), RS_SUCCESS, 200);
    }
    catch (...)
    {
        return detailFailure(PatientStatus::databaseFailure(), 500);
    }
}

PatientOperationResult PatService::add(const PatientAddDTO::Wrapper& dto)
{
    PatientDAO dao;
    TransactionManager transaction(&dao);
    try
    {
        const PayloadDTO* payload = dto->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return operationFailure(PatientStatus::forbidden(), 403);
        }
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (userId.empty() || tenantId.empty())
        {
            transaction.rollback();
            return operationFailure(PatientStatus::forbidden(), 403);
        }
        const std::string organizationId = valueOrEmpty(dto->organizationId);
        if (!dao.organizationExists(tenantId, organizationId))
        {
            transaction.rollback();
            return operationFailure(PatientStatus::organizationNotFound(), 400);
        }

        const std::string id = std::to_string(SnowFlake::getInstance()->nextId());
        const std::string busNo = dto->busNo && !dto->busNo->empty()
            ? dto->busNo->c_str()
            : "PA" + id;
        if (dao.existsBusNo(tenantId, busNo))
        {
            transaction.rollback();
            return operationFailure(PatientStatus::duplicateBusNo(), 409);
        }
        const std::string idCard = valueOrEmpty(dto->idCard);
        if (!idCard.empty() && dao.existsIdCard(tenantId, idCard))
        {
            transaction.rollback();
            return operationFailure(PatientStatus::duplicateIdCard(), 409);
        }

        PatientDO data;
        data.setId(id);
        data.setTenantId(tenantId);
        data.setCreatedAt(SimpleDateTimeFormat::format());
        data.setCreatedBy(userId);
        data.setIsDeleted(0);
        data.setVersion(0);
        data.setBusNo(busNo);
        data.setName(dto->name->c_str());
        data.setGender(dto->genderEnum->c_str());
        data.setStatusEnum(dto->statusEnum->c_str());
        data.setActiveFlag(dto->activeFlag.getValue(true) ? 1 : 0);
        data.setTempFlag(dto->tempFlag.getValue(false) ? 1 : 0);
        data.setOrganizationId(organizationId);

        setOptionalString(dto->nameJson, [&](const std::string& value) { data.setNameJson(value); });
        setOptionalString(dto->birthDate, [&](const std::string& value) { data.setBirthDate(value); });
        setOptionalString(dto->idCard, [&](const std::string& value) { data.setIdCard(value); });
        setOptionalString(dto->maritalStatusEnum, [&](const std::string& value) { data.setMaritalStatusEnum(value); });
        setOptionalString(dto->prfsEnum, [&](const std::string& value) { data.setPrfsEnum(value); });
        setOptionalString(dto->phone, [&](const std::string& value) { data.setPhone(value); });
        setOptionalString(dto->address, [&](const std::string& value) { data.setAddress(value); });
        setOptionalString(dto->addressProvince, [&](const std::string& value) { data.setAddressProvince(value); });
        setOptionalString(dto->addressCity, [&](const std::string& value) { data.setAddressCity(value); });
        setOptionalString(dto->addressDistrict, [&](const std::string& value) { data.setAddressDistrict(value); });
        setOptionalString(dto->addressStreet, [&](const std::string& value) { data.setAddressStreet(value); });
        setOptionalString(dto->addressExtendJson, [&](const std::string& value) { data.setAddressExtendJson(value); });
        setOptionalString(dto->nationalityCode, [&](const std::string& value) { data.setNationalityCode(value); });
        setOptionalString(dto->pyStr, [&](const std::string& value) { data.setPyStr(value); });
        setOptionalString(dto->wbStr, [&](const std::string& value) { data.setWbStr(value); });
        if (dto->bloodTypeAbo) data.setBloodTypeAbo(dto->bloodTypeAbo.getValue(0));
        if (dto->bloodTypeRh) data.setBloodTypeRh(dto->bloodTypeRh.getValue(0));
        setOptionalString(dto->workCompany, [&](const std::string& value) { data.setWorkCompany(value); });
        setOptionalString(dto->nativePlace, [&](const std::string& value) { data.setNativePlace(value); });
        setOptionalString(dto->countryCode, [&](const std::string& value) { data.setCountryCode(value); });
        setOptionalString(dto->linkName, [&](const std::string& value) { data.setLinkName(value); });
        if (dto->linkRelationCode) data.setLinkRelationCode(dto->linkRelationCode.getValue(0));
        setOptionalString(dto->linkTelcom, [&](const std::string& value) { data.setLinkTelcom(value); });
        setOptionalString(dto->linkJsons, [&](const std::string& value) { data.setLinkJsons(value); });

        if (dao.insertPatient(data) != 1)
        {
            transaction.rollback();
            return operationFailure(PatientStatus::databaseFailure(), 500);
        }
        transaction.commit();
        return operationSuccess(id);
    }
    catch (...)
    {
        transaction.rollback();
        return operationFailure(PatientStatus::databaseFailure(), 500);
    }
}

PatientOperationResult PatService::modify(const PatientUpdateDTO::Wrapper& dto)
{
    PatientDAO dao;
    TransactionManager transaction(&dao);
    try
    {
        const PayloadDTO* payload = dto->getPayload();
        if (!payload)
        {
            transaction.rollback();
            return operationFailure(PatientStatus::forbidden(), 403);
        }
        const std::string userId = payload->getId();
        const std::string tenantId = dao.selectTenantIdByUserId(userId);
        if (userId.empty() || tenantId.empty())
        {
            transaction.rollback();
            return operationFailure(PatientStatus::forbidden(), 403);
        }

        const std::string id = dto->id.getValue("");
        auto current = dao.selectByIdForUpdate(id);
        if (!current)
        {
            transaction.rollback();
            return operationFailure(PatientStatus::notFound(), 404);
        }
        if (!current->getTenantIdPtr() || current->getTenantId() != tenantId)
        {
            transaction.rollback();
            return operationFailure(PatientStatus::forbidden(), 403);
        }
        if (current->getVersion() != dto->version.getValue(-1))
        {
            transaction.rollback();
            return operationFailure(PatientStatus::updateConflict(), 409);
        }

        const std::string effectiveBusNo = dto->busNo ? dto->busNo->c_str() : current->getBusNo();
        if (dao.existsBusNo(tenantId, effectiveBusNo, id))
        {
            transaction.rollback();
            return operationFailure(PatientStatus::duplicateBusNo(), 409);
        }
        const std::string effectiveIdCard = dto->idCard
            ? dto->idCard->c_str()
            : (current->getIdCardPtr() ? current->getIdCard() : "");
        if (!effectiveIdCard.empty() && dao.existsIdCard(tenantId, effectiveIdCard, id))
        {
            transaction.rollback();
            return operationFailure(PatientStatus::duplicateIdCard(), 409);
        }
        const std::string effectiveOrganizationId = dto->organizationId
            ? dto->organizationId->c_str()
            : current->getOrganizationId();
        if (!dao.organizationExists(tenantId, effectiveOrganizationId))
        {
            transaction.rollback();
            return operationFailure(PatientStatus::organizationNotFound(), 400);
        }

        if (dao.updatePatch(*dto.get(), tenantId, userId, current->getVersion()) != 1)
        {
            transaction.rollback();
            return operationFailure(PatientStatus::updateConflict(), 409);
        }
        transaction.commit();
        return operationSuccess(id);
    }
    catch (...)
    {
        transaction.rollback();
        return operationFailure(PatientStatus::databaseFailure(), 500);
    }
}

bool PatService::removePatient(const std::string& id, const PayloadDTO& payload)
{
    PatientDAO dao;
    const std::string userId = payload.getId();
    const std::string tenantId = dao.selectTenantIdByUserId(userId);
    if (userId.empty() || tenantId.empty()) return false;
    const std::string now = SimpleDateTimeFormat::format();
    return dao.softDeletePatient(id, tenantId, now, userId) > 0;
}
