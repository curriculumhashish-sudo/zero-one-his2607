/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/23

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "DrugService.h"

#include "../../dao/drug/DrugDAO.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "id/UuidFacade.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <set>

namespace
{
constexpr size_t MAX_BATCH_SIZE = 100;
const std::string STATUS_ACTIVE = "ACTIVE";
const std::string STATUS_INACTIVE = "INACTIVE";
const std::string STATUS_DISCARDED = "DISCARDED";
const std::string PRICE_PURCHASE = "PURCHASE";
const std::string PRICE_SALE = "SALE";
const std::string PRICE_LIMIT = "LIMIT";
const std::string DRUG_TABLE = "med_medication_definition";

bool isValidId(const oatpp::String &id)
{
    if (!id)
    {
        return false;
    }
    const std::string value = id.getValue("");
    if (value.empty() || value.size() > 64)
    {
        return false;
    }
    return std::all_of(value.cbegin(), value.cend(), [](unsigned char character)
                       { return std::isalnum(character) != 0 || character == '-' || character == '_'; });
}

std::string operatorId(const PayloadDTO &payload)
{
    if (!payload.getId().empty())
    {
        return payload.getId();
    }
    if (!payload.getUsername().empty())
    {
        return payload.getUsername();
    }
    throw DrugServiceException(DrugServiceError::InvalidInput);
}

std::list<std::string> normalizeIds(const oatpp::List<oatpp::String> &source)
{
    if (!source || source->empty() || source->size() > MAX_BATCH_SIZE)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }

    std::set<std::string> unique;
    for (const auto &id : *source.get())
    {
        if (!isValidId(id))
        {
            throw DrugServiceException(DrugServiceError::InvalidInput);
        }
        unique.emplace(id.getValue(""));
    }
    if (unique.empty())
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    return {unique.begin(), unique.end()};
}

void ensureSameTenant(const std::list<PtrMedMedicationDefinitionDO> &drugs)
{
    if (drugs.empty())
    {
        return;
    }
    const std::string tenantId = drugs.front()->getTenantId();
    for (const auto &drug : drugs)
    {
        if (!drug || drug->getTenantId() != tenantId)
        {
            throw DrugServiceException(DrugServiceError::CrossTenant);
        }
    }
}

void ensureTenantAccess(DrugDAO &dao, const PayloadDTO &payload, const std::string &tenantId)
{
    if (!dao.hasTenantAccess(payload.getId(), payload.getUsername(), tenantId))
    {
        throw DrugServiceException(DrugServiceError::CrossTenant);
    }
}

void fillEditableDrug(MedMedicationDefinitionDO &data, const DrugModifyDTO::Wrapper &dto,
                      const std::string &updatedBy,
                      const std::string &updatedAt)
{
    ZO_STAR_DOMAIN_DTO_TO_DO(
        data, dto, OrgId, orgId, BusNo, busNo, Name, name, DomainEnum, domainEnum, MedVersion, version, NameEn, nameEn,
        PyStr, pyStr, WbStr, wbStr, CategoryCode, categoryCode, MerchandiseName, merchandiseName, MerchandisePyStr,
        merchandisePyStr, MerchandiseWbStr, merchandiseWbStr, PurchaseUnitCode, purchaseUnitCode, SplitUnitCode,
        splitUnitCode, ComprisedText, comprisedText, Ingredient, ingredient, DoseFrom, doseFrom, ApprovalNumber,
        approvalNumber, YbMatchFlag, ybMatchFlag, YbNo, ybNo, PharmacologyCategoryCode, pharmacologyCategoryCode,
        SkinTestFlag, skinTestFlag, InjectFlag, injectFlag, ManufacturerId, manufacturerId, ManufacturerText,
        manufacturerText, SupplyId, supplyId, RestrictedFlag, restrictedFlag, RestrictedScope, restrictedScope,
        ChildrenFlag, childrenFlag);
    ZO_STAR_DOMAIN_DTO_TO_DO(
        data, dto, Characteristic, characteristic, ItemMinQuantity, itemMinQuantity, ItemMaxQuantity, itemMaxQuantity,
        NationalDrugCode, nationalDrugCode, PartAttributeEnum, partAttributeEnum, AntibioticCode, antibioticCode,
        RestrictedEnum, restrictedEnum, SelfFlag, selfFlag, AntibioticFlag, antibioticFlag, BasicFlag, basicFlag,
        ThoPartAttributeEnum, thoPartAttributeEnum, ChrgitmLv, chrgitmLv, RxFlag, rxFlag, DosageInstruction,
        dosageInstruction, Drug69Code, drug69Code, TotalVolume, totalVolume, DoseFormCode, doseFormCode, DoseUnitCode,
        doseUnitCode, PackageUnitCode, packageUnitCode, StockUnitCode, stockUnitCode, MethodCode, methodCode, RateCode,
        rateCode, Dose, dose, MaxUnit, maxUnit, UsageLimit, usageLimit, LocationId, locationId, IngredientItem,
        ingredientItem, MedicationDefinition, definition, DddCode, dddCode, DddUnitCode, dddUnitCode);
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, MinRateCode, minRateCode, MaxRateCode, maxRateCode, DoseUnitRatio, doseUnitRatio,
                             PurchaseUnitRatio, purchaseUnitRatio, StockUnitRatio, stockUnitRatio, PackageUnitRatio,
                             packageUnitRatio);
    if (dto->ybMatchFlag != nullptr)
    {
        data.setYbMatchFlag(dto->ybMatchFlag.getValue(false));
    }
    if (dto->skinTestFlag != nullptr)
    {
        data.setSkinTestFlag(dto->skinTestFlag.getValue(false));
    }
    if (dto->injectFlag != nullptr)
    {
        data.setInjectFlag(dto->injectFlag.getValue(false));
    }
    if (dto->restrictedFlag != nullptr)
    {
        data.setRestrictedFlag(dto->restrictedFlag.getValue(false));
    }
    if (dto->childrenFlag != nullptr)
    {
        data.setChildrenFlag(dto->childrenFlag.getValue(false));
    }
    if (dto->selfFlag != nullptr)
    {
        data.setSelfFlag(dto->selfFlag.getValue(false));
    }
    if (dto->antibioticFlag != nullptr)
    {
        data.setAntibioticFlag(dto->antibioticFlag.getValue(false));
    }
    if (dto->basicFlag != nullptr)
    {
        data.setBasicFlag(dto->basicFlag.getValue(false));
    }
    if (dto->rxFlag != nullptr)
    {
        data.setRxFlag(dto->rxFlag.getValue(false));
    }
    data.setId(dto->id.getValue(""));
    data.setUpdatedBy(updatedBy);
    data.setUpdatedAt(updatedAt);
}

void fillBasePrice(AdmChargeItemDefinitionDO &price, const DrugModifyDTO::Wrapper &dto,
                   const PtrMedMedicationDefinitionDO &drug,
                   const std::string &priceType, double priceValue, const std::string &createdBy,
                   const std::string &createdAt, UuidFacade &uuidFacade)
{
    price.setId(uuidFacade.genUuid());
    price.setTenantId(drug->getTenantId());
    price.setCreatedAt(createdAt);
    price.setUpdatedAt(createdAt);
    price.setCreatedBy(createdBy);
    price.setUpdatedBy(createdBy);
    price.setIsDeleted(false);
    price.setVersion(0);
    price.setInstanceTable(DRUG_TABLE);
    price.setInstanceId(drug->getId());
    price.setChargeName(dto->name.getValue(""));
    price.setStatusEnum(STATUS_ACTIVE);
    const std::string orgId = dto->orgId ? dto->orgId.getValue("") : drug->getOrgId();
    if (!orgId.empty())
    {
        price.setOrgId(orgId);
    }
    price.setTypeCode(dto->typeCode.getValue(""));
    if (dto->ybType)
    {
        price.setYbType(dto->ybType.getValue(""));
    }
    price.setPriceTypeCode(priceType);
    price.setPriceValue(priceValue);
}

DrugModifyResultDTO::Wrapper toResultDTO(const PtrMedMedicationDefinitionDO &drug,
                                        const std::list<PtrAdmChargeItemDefinitionDO> &prices)
{
    auto dto = DrugModifyResultDTO::createShared();
    ZO_STAR_DOMAIN_DO_TO_DTO_1(
        dto, drug, id, Id, orgId, OrgId, busNo, BusNo, name, Name, domainEnum, DomainEnum, version, MedVersion, nameEn,
        NameEn, pyStr, PyStr, wbStr, WbStr, categoryCode, CategoryCode, merchandiseName, MerchandiseName,
        merchandisePyStr, MerchandisePyStr, merchandiseWbStr, MerchandiseWbStr, purchaseUnitCode, PurchaseUnitCode,
        splitUnitCode, SplitUnitCode, comprisedText, ComprisedText, ingredient, Ingredient, doseFrom, DoseFrom,
        approvalNumber, ApprovalNumber, ybMatchFlag, YbMatchFlag, ybNo, YbNo, pharmacologyCategoryCode,
        PharmacologyCategoryCode, skinTestFlag, SkinTestFlag, injectFlag, InjectFlag, manufacturerId, ManufacturerId,
        manufacturerText, ManufacturerText);
    ZO_STAR_DOMAIN_DO_TO_DTO_1(
        dto, drug, supplyId, SupplyId, restrictedFlag, RestrictedFlag, restrictedScope, RestrictedScope, childrenFlag,
        ChildrenFlag, characteristic, Characteristic, itemMinQuantity, ItemMinQuantity, itemMaxQuantity,
        ItemMaxQuantity, nationalDrugCode, NationalDrugCode, partAttributeEnum, PartAttributeEnum, antibioticCode,
        AntibioticCode, restrictedEnum, RestrictedEnum, selfFlag, SelfFlag, antibioticFlag, AntibioticFlag, basicFlag,
        BasicFlag, thoPartAttributeEnum, ThoPartAttributeEnum, chrgitmLv, ChrgitmLv, rxFlag, RxFlag,
        dosageInstruction, DosageInstruction, drug69Code, Drug69Code, totalVolume, TotalVolume, doseFormCode,
        DoseFormCode, doseUnitCode, DoseUnitCode, packageUnitCode, PackageUnitCode, stockUnitCode, StockUnitCode,
        methodCode, MethodCode, rateCode, RateCode);
    ZO_STAR_DOMAIN_DO_TO_DTO_1(
        dto, drug, dose, Dose, maxUnit, MaxUnit, usageLimit, UsageLimit, locationId, LocationId, ingredientItem,
        IngredientItem, definition, MedicationDefinition, dddCode, DddCode, dddUnitCode, DddUnitCode, minRateCode,
        MinRateCode, maxRateCode, MaxRateCode, doseUnitRatio, DoseUnitRatio, purchaseUnitRatio, PurchaseUnitRatio,
        stockUnitRatio, StockUnitRatio, packageUnitRatio, PackageUnitRatio, statusEnum, StatusEnum, activeFlag,
        ActiveFlag);
    dto->drugDefId = drug->getId();
    dto->specification = drug->getTotalVolume();
    dto->purchaseUnitCodeDictText = nullptr;
    dto->doseFromDictText = nullptr;
    dto->antibioticCodeDictText = nullptr;
    dto->typeCodeDictText = nullptr;
    dto->doseUnitCodeDictText = nullptr;
    dto->categoryCodeDictText = nullptr;
    dto->packageUnitCodeDictText = nullptr;
    dto->locationIdDictText = nullptr;
    dto->supplyIdDictText = nullptr;
    dto->potencyFlag = nullptr;
    dto->unitBands = nullptr;
    dto->typeCode = nullptr;
    dto->ybType = nullptr;
    dto->statusEnumText =
        drug->getStatusEnum() == STATUS_ACTIVE ? ZH_WORDS_GETTER("drug.example.statusEnabled")
                                              : ZH_WORDS_GETTER("drug.example.statusDisabled");
    dto->activeFlagText =
        drug->getActiveFlag() ? ZH_WORDS_GETTER("drug.example.statusEnabled")
                              : ZH_WORDS_GETTER("drug.example.statusDisabled");

    dto->purchasePrice = nullptr;
    dto->retailPrice = nullptr;
    dto->maximumRetailPrice = nullptr;
    for (const auto &price : prices)
    {
        if (!dto->typeCode && !price->getTypeCode().empty())
        {
            dto->typeCode = price->getTypeCode();
        }
        if (!dto->ybType && !price->getYbType().empty())
        {
            dto->ybType = price->getYbType();
        }
        if (price->getPriceTypeCode() == PRICE_PURCHASE)
        {
            dto->purchasePrice = price->getPriceValue();
        }
        else if (price->getPriceTypeCode() == PRICE_SALE)
        {
            dto->retailPrice = price->getPriceValue();
        }
        else if (price->getPriceTypeCode() == PRICE_LIMIT)
        {
            dto->maximumRetailPrice = price->getPriceValue();
        }
    }
    return dto;
}
} // namespace

DrugServiceException::DrugServiceException(DrugServiceError errorCode)
    : std::runtime_error("drug service operation failed"), error(errorCode)
{
}

DrugServiceError DrugServiceException::getError() const noexcept
{
    return error;
}

DrugModifyResultDTO::Wrapper DrugService::updateDrug(const DrugModifyDTO::Wrapper &dto, const PayloadDTO &payload)
{
    if (!dto || !isValidId(dto->id))
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }

    DrugDAO dao;
    const char *stage = "transaction";
    try
    {
        TransactionManager transaction(&dao);
        stage = "select-drug";
        const auto current = dao.selectById(dto->id.getValue(""), true);
        if (!current)
        {
            throw DrugServiceException(DrugServiceError::NotFound);
        }
        ensureTenantAccess(dao, payload, current->getTenantId());
        if (current->getStatusEnum() == STATUS_DISCARDED)
        {
            throw DrugServiceException(DrugServiceError::InvalidStatus);
        }

        const std::string auditOperator = operatorId(payload);
        const std::string operationTime = SimpleDateTimeFormat::format();
        MedMedicationDefinitionDO data;
        stage = "map-request";
        fillEditableDrug(data, dto, auditOperator, operationTime);
        stage = "update-drug";
        if (dao.updateEditableByVersion(data, current->getTenantId(), current->getVersion()) != 1)
        {
            throw DrugServiceException(DrugServiceError::ConcurrentModification);
        }

        stage = "expire-prices";
        dao.softDeleteBasePrices(current->getId(), current->getTenantId(), auditOperator, operationTime);
        UuidFacade uuidFacade;
        size_t expectedPriceCount = 0;
        auto insertPrice = [&](const std::string &priceType, const oatpp::Float64 &value)
        {
            if (!value)
            {
                return;
            }
            AdmChargeItemDefinitionDO price;
            stage = "map-price";
            fillBasePrice(price, dto, current, priceType, value.getValue(0.0), auditOperator, operationTime,
                          uuidFacade);
            stage = "insert-price";
            if (dao.insertBasePrice(price) != 1)
            {
                throw DrugServiceException(DrugServiceError::DatabaseFailure);
            }
            ++expectedPriceCount;
        };
        insertPrice(PRICE_PURCHASE, dto->purchasePrice);
        insertPrice(PRICE_SALE, dto->retailPrice);
        insertPrice(PRICE_LIMIT, dto->maximumRetailPrice);

        stage = "reload-result";
        const auto updated = dao.selectById(current->getId());
        const auto prices = dao.selectBasePrices(current->getId(), current->getTenantId());
        if (!updated || prices.size() != expectedPriceCount)
        {
            throw DrugServiceException(DrugServiceError::DatabaseFailure);
        }
        stage = "map-result";
        auto result = toResultDTO(updated, prices);
        stage = "commit";
        transaction.commit();
        return result;
    }
    catch (...)
    {
        const auto originalError = std::current_exception();
        try
        {
            std::rethrow_exception(originalError);
        }
        catch (const std::exception &error)
        {
            std::cerr << "DrugService::updateDrug failed at " << stage << ": " << error.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "DrugService::updateDrug failed at " << stage << ": unknown exception" << std::endl;
        }
        try
        {
            dao.getSqlSession()->rollbackTransaction();
        }
        catch (...)
        {
        }
        dao.releaseSqlSession();
        std::rethrow_exception(originalError);
    }
}

void DrugService::removeDrugs(const DrugRemoveDTO::Wrapper &dto, const PayloadDTO &payload)
{
    const auto ids = normalizeIds(dto ? dto->ids : nullptr);
    DrugDAO dao;
    try
    {
        TransactionManager transaction(&dao);
        const auto drugs = dao.selectStatesForUpdate(ids);
        if (drugs.size() != ids.size())
        {
            throw DrugServiceException(DrugServiceError::NotFound);
        }
        ensureSameTenant(drugs);
        ensureTenantAccess(dao, payload, drugs.front()->getTenantId());
        for (const auto &drug : drugs)
        {
            if (drug->getStatusEnum() != STATUS_INACTIVE || drug->getActiveFlag())
            {
                throw DrugServiceException(DrugServiceError::InvalidStatus);
            }
        }

        const auto referenceCheck = dao.selectReferenceCheck(ids);
        if (!referenceCheck)
        {
            throw DrugServiceException(DrugServiceError::DatabaseFailure);
        }
        if (referenceCheck->referenced)
        {
            throw DrugServiceException(DrugServiceError::Referenced);
        }

        const std::string auditOperator = operatorId(payload);
        const std::string operationTime = SimpleDateTimeFormat::format();
        dao.softDeleteAllPrices(ids, drugs.front()->getTenantId(), auditOperator, operationTime);
        const auto activePricePresence = dao.selectActivePricePresence(ids, drugs.front()->getTenantId());
        if (!activePricePresence || activePricePresence->satisfied)
        {
            throw DrugServiceException(DrugServiceError::DatabaseFailure);
        }
        if (dao.softDeleteDrugs(ids, drugs.front()->getTenantId(), auditOperator, operationTime) !=
            static_cast<int>(ids.size()))
        {
            throw DrugServiceException(DrugServiceError::ConcurrentModification);
        }
        transaction.commit();
    }
    catch (...)
    {
        const auto originalError = std::current_exception();
        try
        {
            dao.getSqlSession()->rollbackTransaction();
        }
        catch (...)
        {
        }
        dao.releaseSqlSession();
        std::rethrow_exception(originalError);
    }
}

void DrugService::enableDisableDrugs(const DrugEnableDisableDTO::Wrapper &dto, const PayloadDTO &payload)
{
    if (!dto || dto->activeFlag == nullptr)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    const auto ids = normalizeIds(dto->ids);
    const bool enable = dto->activeFlag.getValue(false);
    const std::string expectedStatus = enable ? STATUS_INACTIVE : STATUS_ACTIVE;
    const std::string targetStatus = enable ? STATUS_ACTIVE : STATUS_INACTIVE;

    DrugDAO dao;
    try
    {
        TransactionManager transaction(&dao);
        const auto drugs = dao.selectStatesForUpdate(ids);
        if (drugs.size() != ids.size())
        {
            throw DrugServiceException(DrugServiceError::NotFound);
        }
        ensureSameTenant(drugs);
        ensureTenantAccess(dao, payload, drugs.front()->getTenantId());
        for (const auto &drug : drugs)
        {
            if (drug->getStatusEnum() != expectedStatus || drug->getActiveFlag() == enable)
            {
                throw DrugServiceException(DrugServiceError::InvalidStatus);
            }
        }
        if (enable)
        {
            const auto readiness = dao.selectEnableReadiness(ids, drugs.front()->getTenantId());
            if (!readiness)
            {
                throw DrugServiceException(DrugServiceError::DatabaseFailure);
            }
            if (!readiness->satisfied)
            {
                throw DrugServiceException(DrugServiceError::InvalidInput);
            }
        }

        const std::string auditOperator = operatorId(payload);
        const std::string operationTime = SimpleDateTimeFormat::format();
        if (dao.updateActiveStatus(ids, drugs.front()->getTenantId(), expectedStatus, targetStatus, enable,
                                   auditOperator, operationTime) !=
            static_cast<int>(ids.size()))
        {
            throw DrugServiceException(DrugServiceError::ConcurrentModification);
        }
        transaction.commit();
    }
    catch (...)
    {
        const auto originalError = std::current_exception();
        try
        {
            dao.getSqlSession()->rollbackTransaction();
        }
        catch (...)
        {
        }
        dao.releaseSqlSession();
        std::rethrow_exception(originalError);
    }
}
