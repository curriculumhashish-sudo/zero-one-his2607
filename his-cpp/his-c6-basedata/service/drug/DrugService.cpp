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
#include "domain/dto/drug/DrugSelectDTO.h"
#include "../../dao/common/DrugEnumDictionaryDAO.h"
#include "../../dao/drug/DrugDAO.h"
#include "../../domain/DrugConstants.h"
#include "domain/dto/drug/DrugSelectDTO.h"
#include "SimpleDateTimeFormat.h"
#include "TransactionManager.h"
#include "id/UuidFacade.h"
#include <algorithm>
#include <cctype>
#include <exception>
#include <set>

static bool isValidId(const oatpp::String &id)
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

static std::string operatorId(const PayloadDTO &payload)
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

static std::list<std::string> normalizeIds(const oatpp::List<oatpp::String> &source)
{
    if (!source || source->empty() || source->size() > DrugConstants::MAX_BATCH_SIZE)
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

static void ensureSameTenant(const std::list<PtrMedMedicationDefinitionDO> &drugs)
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

static void ensureTenantAccess(DrugDAO &dao, const PayloadDTO &payload, const std::string &tenantId)
{
    if (!dao.hasTenantAccess(payload.getId(), payload.getUsername(), tenantId))
    {
        throw DrugServiceException(DrugServiceError::CrossTenant);
    }
}

static void fillEditableDrug(MedMedicationDefinitionDO &data, const DrugModifyDTO::Wrapper &dto,
                             const std::string &updatedBy,
                             const std::string &updatedAt)
{
    ZO_STAR_DOMAIN_DTO_TO_DO(
        data, dto, BusNo, busNo, Name, name, DomainEnum, domainEnum, MedVersion, medVersion, NameEn, nameEn,
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
    ZO_STAR_DOMAIN_DTO_TO_DO(
        data, dto, YbMatchFlag, ybMatchFlag, SkinTestFlag, skinTestFlag, InjectFlag, injectFlag, RestrictedFlag,
        restrictedFlag, ChildrenFlag, childrenFlag, SelfFlag, selfFlag, AntibioticFlag, antibioticFlag, BasicFlag,
        basicFlag, RxFlag, rxFlag, Id, id);
    // Keep stored default unit codes because this request does not carry them.
    data.setUpdatedBy(updatedBy);
    data.setUpdatedAt(updatedAt);
}

static void fillBasePrice(AdmChargeItemDefinitionDO &price, const DrugModifyDTO::Wrapper &dto,
                          const PtrMedMedicationDefinitionDO &drug,
                          const std::string &priceType, double priceValue, const std::string &createdBy,
                          const std::string &createdAt, UuidFacade &uuidFacade)
{
    const std::string priceId = uuidFacade.genUuid();
    if (priceId.empty())
    {
        throw DrugServiceException(DrugServiceError::DatabaseFailure);
    }
    price.setId(priceId);
    price.setTenantId(drug->getTenantId());
    price.setCreatedAt(createdAt);
    price.setUpdatedAt(createdAt);
    price.setCreatedBy(createdBy);
    price.setUpdatedBy(createdBy);
    price.setIsDeleted(false);
    price.setVersion(0);
    price.setInstanceTable(drug->getTable());
    price.setInstanceId(drug->getId());
    ZO_STAR_DOMAIN_DTO_TO_DO(price, dto, ChargeName, name, TypeCode, typeCode, YbType, ybType);
    price.setStatusEnum(DrugConstants::STATUS_ACTIVE);
    // 机构归属以数据库记录为准。
    const std::string orgId = drug->getOrgId();
    if (!orgId.empty())
    {
        price.setOrgId(orgId);
    }
    price.setPriceTypeCode(priceType);
    price.setPriceValue(priceValue);
}

static void addUnitBand(oatpp::List<DrugUnitBandDTO::Wrapper> bands,
                        const std::string &kind,
                        const std::string &unitCode,
                        const std::string &unitName,
                        double ratioToSplit,
                        const oatpp::Float64 &unitPrice = nullptr)
{
    if (unitCode.empty() || ratioToSplit <= 0.0)
    {
        return;
    }

    auto band = DrugUnitBandDTO::createShared();
    band->bandKind = kind;
    band->unitCode = unitCode;
    band->unitName = unitName.empty() ? nullptr : oatpp::String(unitName);
    band->ratioToSplit = ratioToSplit;
    band->unitPrice = unitPrice;
    bands->push_back(band);
}

static DrugModifyResultDTO::Wrapper toResultDTO(const PtrMedMedicationDefinitionDO &drug,
                                                const std::list<PtrAdmChargeItemDefinitionDO> &prices,
                                                const PtrDrugModifyTextResult &texts,
                                                const std::string &statusText,
                                                const std::string &activeFlagText)
{
    auto dto = DrugModifyResultDTO::createShared();
    ZO_STAR_DOMAIN_DO_TO_DTO_1(
        dto, drug, id, Id, orgId, OrgId, busNo, BusNo, name, Name, domainEnum, DomainEnum, medVersion, MedVersion, nameEn,
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
        stockUnitRatio, StockUnitRatio, packageUnitRatio, PackageUnitRatio, activeFlag, ActiveFlag);
    ZO_STAR_DOMAIN_DO_TO_DTO_1(dto, drug, medicationDefId, Id, specification, TotalVolume);
    dto->purchaseUnitCodeDictText = texts->purchaseUnitName;
    dto->doseFromDictText = texts->doseFromName.empty() ? nullptr : oatpp::String(texts->doseFromName);
    dto->antibioticCodeDictText = texts->antibioticName.empty() ? nullptr : oatpp::String(texts->antibioticName);
    dto->typeCodeDictText = texts->typeName;
    dto->doseUnitCodeDictText = texts->doseUnitName;
    dto->categoryCodeDictText = texts->categoryName;
    dto->packageUnitCodeDictText = texts->packageUnitName;
    dto->locationIdDictText = texts->locationName.empty() ? nullptr : oatpp::String(texts->locationName);
    dto->supplyIdDictText = texts->supplierName;
    dto->potencyFlag = nullptr;
    dto->unitBands = oatpp::List<DrugUnitBandDTO::Wrapper>::createShared();
    dto->typeCode = nullptr;
    dto->ybType = nullptr;
    if (drug->getStatusEnum() == DrugConstants::STATUS_ACTIVE)
    {
        dto->statusEnum = DrugConstants::API_STATUS_ACTIVE;
    }
    else if (drug->getStatusEnum() == DrugConstants::STATUS_INACTIVE)
    {
        dto->statusEnum = DrugConstants::API_STATUS_INACTIVE;
    }
    else
    {
        dto->statusEnum = DrugConstants::API_STATUS_DISCARDED;
    }
    dto->statusEnumText = statusText;
    dto->activeFlagText = activeFlagText;

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
        if (price->getPriceTypeCode() == DrugConstants::PRICE_TYPE_PURCHASE)
        {
            dto->purchasePrice = price->getPriceValue();
        }
        else if (price->getPriceTypeCode() == DrugConstants::PRICE_TYPE_SALE)
        {
            dto->retailPrice = price->getPriceValue();
        }
        else if (price->getPriceTypeCode() == DrugConstants::PRICE_TYPE_LIMIT)
        {
            dto->maximumRetailPrice = price->getPriceValue();
        }
    }

    addUnitBand(dto->unitBands, DrugConstants::UNIT_BAND_SPLIT, drug->getSplitUnitCode(), texts->splitUnitName,
                DrugConstants::SPLIT_UNIT_RATIO);
    addUnitBand(dto->unitBands, DrugConstants::UNIT_BAND_DOSE, drug->getDoseUnitCode(), texts->doseUnitName,
                drug->getDoseUnitRatio());
    addUnitBand(dto->unitBands, DrugConstants::UNIT_BAND_PURCHASE, drug->getPurchaseUnitCode(),
                texts->purchaseUnitName,
                drug->getPurchaseUnitRatio(),
                dto->purchasePrice);
    addUnitBand(dto->unitBands, DrugConstants::UNIT_BAND_STOCK, drug->getStockUnitCode(),
                texts->stockUnitName,
                drug->getStockUnitRatio());
    addUnitBand(dto->unitBands, DrugConstants::UNIT_BAND_PACKAGE, drug->getPackageUnitCode(),
                texts->packageUnitName,
                drug->getPackageUnitRatio(),
                dto->retailPrice);
    return dto;
}

DrugServiceException::DrugServiceException(DrugServiceError errorCode)
    : std::runtime_error("drug service operation failed"), error(errorCode)
{
}

DrugServiceError DrugServiceException::getError() const noexcept
{
    return error;
}

DrugModifyResultDTO::Wrapper DrugService::updateDrug(const DrugModifyDTO::Wrapper &dto)
{
    // 校验请求和登录信息。
    if (!dto || !isValidId(dto->id) || dto->getPayload() == nullptr)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    const PayloadDTO &payload = *dto->getPayload();

    DrugDAO dao;
    const char *stage = "transaction";
    try
    {
        // 锁定记录并校验租户和状态。
        TransactionManager transaction(&dao);
        stage = "select-drug";
        const auto current = dao.selectById(dto->id.getValue(""), true);
        if (!current)
        {
            throw DrugServiceException(DrugServiceError::NotFound);
        }
        ensureTenantAccess(dao, payload, current->getTenantId());
        if (dto->orgId && dto->orgId.getValue("") != current->getOrgId())
        {
            throw DrugServiceException(DrugServiceError::InvalidInput);
        }
        if (current->getStatusEnum() == DrugConstants::STATUS_DISCARDED)
        {
            throw DrugServiceException(DrugServiceError::InvalidStatus);
        }

        // Validate both split rules with the MongoDB dictionary.
        stage = "validate-enums";
        DrugEnumDictionaryDAO drugEnumDictionaryDAO;
        const std::vector<std::string> ruleCodes = {
            dto->partAttributeEnum.getValue(""),
            dto->thoPartAttributeEnum.getValue("")};
        const auto ruleDisplays = drugEnumDictionaryDAO.findActiveDisplays(
            DrugEnumDictionaryDAO::ORDER_CALC_RULE_ENUM,
            ruleCodes);
        if (ruleDisplays.find(ruleCodes[0]) == ruleDisplays.end() ||
            ruleDisplays.find(ruleCodes[1]) == ruleDisplays.end())
        {
            throw DrugServiceException(DrugServiceError::InvalidInput);
        }

        const std::string auditOperator = operatorId(payload);
        const std::string operationTime = SimpleDateTimeFormat::format();
        MedMedicationDefinitionDO data;
        // 将 DTO 转为可修改 DO。
        stage = "map-request";
        fillEditableDrug(data, dto, auditOperator, operationTime);
        stage = "update-drug";
        if (dao.updateEditableByVersion(data, current->getTenantId(), current->getVersion()) != 1)
        {
            throw DrugServiceException(DrugServiceError::ConcurrentModification);
        }

        // 同一事务内替换基础价格。
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
        insertPrice(DrugConstants::PRICE_TYPE_PURCHASE, dto->purchasePrice);
        insertPrice(DrugConstants::PRICE_TYPE_SALE, dto->retailPrice);
        insertPrice(DrugConstants::PRICE_TYPE_LIMIT, dto->maximumRetailPrice);

        // 回读数据并提交事务。
        stage = "reload-result";
        const auto updated = dao.selectById(current->getId());
        const auto prices = dao.selectBasePrices(current->getId(), current->getTenantId());
        if (!updated || prices.size() != expectedPriceCount)
        {
            throw DrugServiceException(DrugServiceError::DatabaseFailure);
        }
        const auto texts = dao.selectModifyResultTexts(current->getId(), current->getTenantId(),
                                                       dto->typeCode.getValue(""));
        if (!texts || texts->purchaseUnitName.empty() || texts->splitUnitName.empty() ||
            texts->doseUnitName.empty() || texts->packageUnitName.empty() || texts->stockUnitName.empty() ||
            texts->typeName.empty() || texts->categoryName.empty() || texts->supplierName.empty())
        {
            throw DrugServiceException(DrugServiceError::InvalidInput);
        }
        const std::string activeStatus = updated->getActiveFlag()
                                             ? DrugConstants::STATUS_ACTIVE
                                             : DrugConstants::STATUS_INACTIVE;
        const auto statusDisplays = drugEnumDictionaryDAO.findActiveDisplays(
            DrugEnumDictionaryDAO::PUBLICATION_STATUS_ENUM,
            {updated->getStatusEnum(), activeStatus});
        const auto statusText = statusDisplays.find(updated->getStatusEnum());
        const auto activeFlagText = statusDisplays.find(activeStatus);
        if (statusText == statusDisplays.end() || activeFlagText == statusDisplays.end())
        {
            throw DrugServiceException(DrugServiceError::DatabaseFailure);
        }
        stage = "map-result";
        auto result = toResultDTO(updated, prices, texts, statusText->second, activeFlagText->second);
        stage = "commit";
        transaction.commit();
        return result;
    }
    catch (...)
    {
        // 回滚并保留原异常。
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

bool DrugService::removeDrugs(const DrugRemoveDTO::Wrapper &dto)
{
    // 校验并整理批量编号。
    if (!dto || dto->getPayload() == nullptr)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    const PayloadDTO &payload = *dto->getPayload();
    const auto ids = normalizeIds(dto ? dto->ids : nullptr);
    DrugDAO dao;
    try
    {
        // 锁定批次并校验租户和状态。
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
            if (drug->getStatusEnum() != DrugConstants::STATUS_INACTIVE || drug->getActiveFlag())
            {
                throw DrugServiceException(DrugServiceError::InvalidStatus);
            }
        }

        // 被业务引用时禁止删除。
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
        // 先删除价格，再删除药品。
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
        return true;
    }
    catch (...)
    {
        // 整批回滚并抛出原异常。
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

bool DrugService::enableDisableDrugs(const DrugEnableDisableDTO::Wrapper &dto)
{
    // 校验目标状态和批量编号。
    if (!dto || dto->activeFlag == nullptr || dto->getPayload() == nullptr)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    const PayloadDTO &payload = *dto->getPayload();
    const auto ids = normalizeIds(dto->ids);
    const bool enable = dto->activeFlag.getValue(false);
    const std::string expectedStatus =
        enable ? DrugConstants::STATUS_INACTIVE : DrugConstants::STATUS_ACTIVE;
    const std::string targetStatus =
        enable ? DrugConstants::STATUS_ACTIVE : DrugConstants::STATUS_INACTIVE;

    DrugDAO dao;
    try
    {
        // 锁定批次并校验原状态。
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
            // 启用前校验主数据、单位和售价。
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
        // 条件更新整批状态。
        if (dao.updateActiveStatus(ids, drugs.front()->getTenantId(), expectedStatus, targetStatus, enable,
                                   auditOperator, operationTime) !=
            static_cast<int>(ids.size()))
        {
            throw DrugServiceException(DrugServiceError::ConcurrentModification);
        }
        transaction.commit();
        return true;
    }
    catch (...)
    {
        // 整批回滚并抛出原异常。
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
DrugListPageDTO::Wrapper DrugService::listAll(const DrugQuery::Wrapper& query)
{
    auto pages = DrugListPageDTO::createShared();
    pages->pageIndex = query->pageIndex;
    pages->pageSize = query->pageSize;

    // 查询符合条件的药品总数。
    DrugDAO dao;
    uint64_t count = dao.count(query);
    if (count <= 0)
    {
        return pages;
    }

    // 分页查询并转换为列表 DTO。
    pages->total = count;
    pages->calcPages();
    std::list<PtrMedMedicationDefinitionDO> result = dao.selectWithPage(query);
    for (const auto &sub : result)
    {
        if (!sub)
        {
            continue;
        }
        auto dto = DrugListDTO::createShared();
        ZO_STAR_DOMAIN_DO_TO_DTO_1(dto, sub, id, Id, code, BusNo, name, Name, guige, TotalVolume,
                                   kinds, CategoryCode, changjia, ManufacturerText);
        const std::string status = sub->getStatusEnum();
        dto->status = status == DrugConstants::STATUS_ACTIVE      ? 1
                      : status == DrugConstants::STATUS_INACTIVE ? 2
                      : status == DrugConstants::STATUS_DISCARDED ? 3
                                                                  : 0;
        pages->addData(dto);
    }
    return pages;

}
DrugSelectPageDTO::Wrapper DrugService::SelectlistAll(const DrugSelectQuery::Wrapper& query)
{
    auto pages = DrugSelectPageDTO::createShared();
    pages->pageIndex = query->pageIndex;
    pages->pageSize = query->pageSize;

    // 查询符合条件的药品总数。
    DrugDAO dao;
    uint64_t count = dao.count2(query);
    if (count <= 0)
    {
        return pages;
    }

    // 分页查询并转换为列表 DTO。
    pages->total = count;
    pages->calcPages();
    std::list<PtrMedMedicationDefinitionDO> result = dao.SElectWithPage(query);
    for (const auto& sub : result)
    {
        if (!sub)
        {
            continue;
        }
        auto dto = DrugSelectDTO::createShared();
        ZO_STAR_DOMAIN_DO_TO_DTO_1(dto, sub, code, Id, name, Name, danwei, StockUnitCode);
        const std::string status = sub->getStatusEnum();
        pages->addData(dto);
    }
    return pages;

}
DrugDetailDTO::Wrapper DrugService::getById(string id)
{
    DrugDAO dao;
    auto res = dao.selectById(id);

    // 未查询到药品时返回空结果。
    if (!res)
    {
        return nullptr;
    }

    // 将药品 DO 转换为详情 DTO。
    auto dto = DrugDetailDTO::createShared();
    ZO_STAR_DOMAIN_DO_TO_DTO_1(dto, res, id, Id, tongyongname, Name, name, MerchandiseName,
                               kinds, CategoryCode, changjia, ManufacturerText, gongyingshang, SupplyId,
                               wenhao, ApprovalNumber, guojiabianma, NationalDrugCode, yibaobianma, YbNo,
                               guige, TotalVolume, danwei, DoseUnitCode, jixin, DoseFormCode,
                               yaopingyinma, PyStr, shangpingyinma, MerchandisePyStr,
                               shangwubima, MerchandiseWbStr, liujiuma, Drug69Code,
                               yaopingstatus, StatusEnum, lingyu, DomainEnum,yibaoduima,YbMatchFlag,xingzhi,Characteristic, beishu, DoseUnitRatio, pishipanbie, SkinTestFlag, zhusheyaopin, InjectFlag, yongliangxianding, UsageLimit, yongfa, MethodCode, pingci, RateCode, dancijiliang, Dose, dancizuida, ItemMaxQuantity, yongfashuoming, DosageInstruction, shiyongfanwei, RestrictedScope
                          );
    ZO_STAR_DOMAIN_DO_TO_DTO_1(dto, res, DDDzhi, DddCode, DDDdanwei, DddUnitCode, caigoudanwei, PurchaseUnitCode, caigoubeishu, PurchaseUnitRatio, menzhengshuxing, DefEncounterUnitCode, kucundanwei, StockUnitCode, kucunbeishu, StockUnitRatio, linshishuxing, DefInhospitalUnitCode, baozhuangbeishu, PackageUnitRatio, baozhuangdanwei, PackageUnitCode, chailingdanwei, SplitUnitCode, selfFlag, SelfFlag, antibioticFlag, AntibioticFlag, basicFlag, BasicFlag,
        activeFlag, ActiveFlag, childrenFlag, ChildrenFlag,
        restrictedFlag, RestrictedFlag, rxFlag, RxFlag);
   
    return dto;

}
string DrugService::saveData(const DrugAddDTO::Wrapper& dto)
{
    DrugDAO dao;
    MedMedicationDefinitionDO entity;

    UuidFacade uuidFacade;
    std::string newId = uuidFacade.genUuid();
    entity.setId(newId);

    entity.setName(dto->name.getValue(""));
    entity.setCategoryCode(dto->kinds.getValue(""));

    entity.setCreatedBy("");
    entity.setCreatedAt(SimpleDateTimeFormat::format());
    
    // 樑扢蘇蚚
    entity.setActiveFlag(true);

    if (dao.insert(entity) > 0) {
        return newId;
    }
    return "";

}
