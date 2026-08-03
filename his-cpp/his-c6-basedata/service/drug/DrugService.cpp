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

static void ensureTenantAccess(DrugDAO &dao, const PayloadDTO &payload, const std::string &tenantId)
{
    if (!dao.hasTenantAccess(payload.getId(), payload.getUsername(), tenantId))
    {
        throw DrugServiceException(DrugServiceError::CrossTenant);
    }
}

static void fillMedicineV2Patch(MedMedicationDefinitionDO &data, const std::string &id,
                                const MedicineUpdateDTO::Wrapper &dto,
                                const std::string &updatedBy, const std::string &updatedAt)
{
    // 仅映射请求中出现的字段，未传字段不会进入 UPDATE。
    data.setId(id);
    ZO_STAR_DOMAIN_DTO_TO_DO(
        data, dto, BusNo, busNo, Name, name, DomainEnum, domainEnum, MedVersion, medVersion, NameEn, nameEn,
        PyStr, pyStr, WbStr, wbStr, CategoryCode, categoryCode, MerchandiseName, merchandiseName,
        MerchandisePyStr, merchandisePyStr, MerchandiseWbStr, merchandiseWbStr, PurchaseUnitCode,
        purchaseUnitCode, SplitUnitCode, splitUnitCode, ComprisedText, comprisedText, Ingredient, ingredient,
        DoseFrom, doseFrom, ApprovalNumber, approvalNumber, YbMatchFlag, ybMatchFlag, YbNo, ybNo,
        PharmacologyCategoryCode, pharmacologyCategoryCode, SkinTestFlag, skinTestFlag, InjectFlag, injectFlag,
        ManufacturerId, manufacturerId, ManufacturerText, manufacturerText, SupplyId, supplyId, RestrictedFlag,
        restrictedFlag, RestrictedScope, restrictedScope);
    ZO_STAR_DOMAIN_DTO_TO_DO(
        data, dto, ChildrenFlag, childrenFlag, Characteristic, characteristic, ItemMinQuantity, itemMinQuantity,
        ItemMaxQuantity, itemMaxQuantity, NationalDrugCode, nationalDrugCode, PartAttributeEnum, partAttributeEnum,
        AntibioticCode, antibioticCode, RestrictedEnum, restrictedEnum, SelfFlag, selfFlag, AntibioticFlag,
        antibioticFlag, BasicFlag, basicFlag, ThoPartAttributeEnum, thoPartAttributeEnum, ChrgitmLv, chrgitmLv,
        RxFlag, rxFlag, DosageInstruction, dosageInstruction, Drug69Code, drug69Code, DoseFormCode, doseFormCode,
        DoseUnitCode, doseUnitCode, PackageUnitCode, packageUnitCode, StockUnitCode, stockUnitCode, MethodCode,
        methodCode, RateCode, rateCode, Dose, dose, MaxUnit, maxUnit, UsageLimit, usageLimit, LocationId, locationId,
        IngredientItem, ingredientItem, MedicationDefinition, definition, DddCode, dddCode, DddUnitCode,
        dddUnitCode);
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto, MinRateCode, minRateCode, MaxRateCode, maxRateCode, DoseUnitRatio,
                             doseUnitRatio, PurchaseUnitRatio, purchaseUnitRatio, StockUnitRatio, stockUnitRatio,
                             PackageUnitRatio, packageUnitRatio);
    if (dto->specification)
    {
        data.setTotalVolume(dto->specification.getValue(""));
    }
    data.setUpdatedBy(updatedBy);
    data.setUpdatedAt(updatedAt);
}

static MedicineVersionItemDTO::Wrapper makeMedicineVersionItem(const std::string &id, int version)
{
    auto item = MedicineVersionItemDTO::createShared();
    item->id = id;
    item->version = version;
    return item;
}

static void addMedicineV2Failure(const MedicineBatchResultDTO::Wrapper &result, const std::string &id,
                                 int code, const std::string &message,
                                 const oatpp::Int32 &currentVersion = nullptr)
{
    auto item = MedicineFailedItemDTO::createShared();
    item->id = id;
    item->code = code;
    item->message = message;
    item->currentVersion = currentVersion;
    result->failedItems->push_back(item);
}

static int medicineV2FailureCode(DrugServiceError error)
{
    switch (error)
    {
    case DrugServiceError::NotFound:
        return MedicineV2BusinessCode::NOT_FOUND;
    case DrugServiceError::CrossTenant:
        return MedicineV2BusinessCode::FORBIDDEN;
    case DrugServiceError::ConcurrentModification:
        return MedicineV2BusinessCode::VERSION_CONFLICT;
    case DrugServiceError::DatabaseFailure:
        return MedicineV2BusinessCode::SERVER_ERROR;
    case DrugServiceError::InvalidInput:
    case DrugServiceError::InvalidStatus:
    case DrugServiceError::Referenced:
    default:
        return MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED;
    }
}

static std::string medicineV2FailureMessage(DrugServiceError error)
{
    switch (error)
    {
    case DrugServiceError::NotFound:
        return ZH_WORDS_GETTER("drug.v2.message.notFound");
    case DrugServiceError::CrossTenant:
        return ZH_WORDS_GETTER("drug.v2.message.forbidden");
    case DrugServiceError::ConcurrentModification:
        return ZH_WORDS_GETTER("drug.v2.message.versionConflict");
    case DrugServiceError::InvalidStatus:
        return ZH_WORDS_GETTER("drug.v2.message.invalidStatus");
    case DrugServiceError::Referenced:
        return ZH_WORDS_GETTER("drug.v2.message.referenced");
    case DrugServiceError::DatabaseFailure:
        return ZH_WORDS_GETTER("drug.v2.message.serverError");
    case DrugServiceError::InvalidInput:
    default:
        return ZH_WORDS_GETTER("drug.v2.message.businessFailed");
    }
}

DrugServiceException::DrugServiceException(DrugServiceError errorCode)
    : std::runtime_error("drug service operation failed"), error(errorCode)
{
}

DrugServiceError DrugServiceException::getError() const noexcept
{
    return error;
}

MedicineVersionItemDTO::Wrapper DrugService::updateMedicineV2(const std::string &id,
                                                               const MedicineUpdateDTO::Wrapper &dto)
{
    if (!isValidId(id) || !dto || dto->getPayload() == nullptr)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    const PayloadDTO &payload = *dto->getPayload();
    DrugDAO dao;
    try
    {
        TransactionManager transaction(&dao);
        const auto current = dao.selectById(id, true);
        if (!current)
        {
            throw DrugServiceException(DrugServiceError::NotFound);
        }
        ensureTenantAccess(dao, payload, current->getTenantId());
        if (current->getStatusEnum() == DrugConstants::STATUS_DISCARDED)
        {
            throw DrugServiceException(DrugServiceError::InvalidStatus);
        }
        if (dto->version.getValue(-1) != current->getVersion())
        {
            throw DrugServiceException(DrugServiceError::ConcurrentModification);
        }

        // 只校验本次实际提交的拆分规则枚举。
        std::vector<std::string> ruleCodes;
        if (dto->partAttributeEnum)
        {
            ruleCodes.emplace_back(dto->partAttributeEnum.getValue(""));
        }
        if (dto->thoPartAttributeEnum)
        {
            ruleCodes.emplace_back(dto->thoPartAttributeEnum.getValue(""));
        }
        if (!ruleCodes.empty())
        {
            DrugEnumDictionaryDAO dictionaryDAO;
            const auto displays = dictionaryDAO.findActiveDisplays(
                DrugEnumDictionaryDAO::ORDER_CALC_RULE_ENUM, ruleCodes);
            if (displays.size() != ruleCodes.size())
            {
                throw DrugServiceException(DrugServiceError::InvalidInput);
            }
        }

        const std::string auditOperator = operatorId(payload);
        const std::string operationTime = SimpleDateTimeFormat::format();
        MedMedicationDefinitionDO data;
        fillMedicineV2Patch(data, id, dto, auditOperator, operationTime);
        if (dao.updateEditableByVersion(data, current->getTenantId(), current->getVersion()) != 1)
        {
            throw DrugServiceException(DrugServiceError::ConcurrentModification);
        }

        // 三种价格分别更新，未传价格保持不变。
        auto updatePrice = [&](const std::string &priceType, const oatpp::Float64 &value)
        {
            if (value &&
                dao.updateBasePriceValue(id, current->getTenantId(), priceType, value.getValue(0.0),
                                         auditOperator, operationTime) != 1)
            {
                throw DrugServiceException(DrugServiceError::InvalidInput);
            }
        };
        updatePrice(DrugConstants::PRICE_TYPE_PURCHASE, dto->purchasePrice);
        updatePrice(DrugConstants::PRICE_TYPE_SALE, dto->retailPrice);
        updatePrice(DrugConstants::PRICE_TYPE_LIMIT, dto->maximumRetailPrice);

        if (dto->financeTypeCode &&
            dao.updateBasePriceFinanceType(id, current->getTenantId(), dto->financeTypeCode.getValue(""),
                                           auditOperator, operationTime) <= 0)
        {
            throw DrugServiceException(DrugServiceError::InvalidInput);
        }
        if (dto->ybType &&
            dao.updateBasePriceInsuranceType(id, current->getTenantId(), dto->ybType.getValue(""),
                                             auditOperator, operationTime) <= 0)
        {
            throw DrugServiceException(DrugServiceError::InvalidInput);
        }
        if (dto->name &&
            dao.updateBasePriceChargeName(id, current->getTenantId(), dto->name.getValue(""),
                                          auditOperator, operationTime) <= 0)
        {
            throw DrugServiceException(DrugServiceError::InvalidInput);
        }

        transaction.commit();
        return makeMedicineVersionItem(id, current->getVersion() + 1);
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

MedicineBatchResultDTO::Wrapper DrugService::updateMedicineStatusV2(
    const MedicineStatusUpdateDTO::Wrapper &dto)
{
    if (!dto || dto->getPayload() == nullptr)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    const PayloadDTO &payload = *dto->getPayload();
    const std::string auditOperator = operatorId(payload);
    const bool enable = dto->status == DrugConstants::STATUS_ACTIVE;
    const std::string expectedStatus =
        enable ? DrugConstants::STATUS_INACTIVE : DrugConstants::STATUS_ACTIVE;
    const std::string targetStatus =
        enable ? DrugConstants::STATUS_ACTIVE : DrugConstants::STATUS_INACTIVE;
    auto result = MedicineBatchResultDTO::createEmpty();

    // 每个药品使用独立事务，失败项不会回滚成功项。
    for (const auto &item : *dto->items.get())
    {
        DrugDAO dao;
        oatpp::Int32 currentVersion = nullptr;
        int failureCode = MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED;
        std::string failureMessage = ZH_WORDS_GETTER("drug.v2.message.businessFailed");
        try
        {
            TransactionManager transaction(&dao);
            const std::string id = item->id.getValue("");
            const auto current = dao.selectById(id, true);
            if (!current)
            {
                throw DrugServiceException(DrugServiceError::NotFound);
            }
            if (!dao.hasTenantAccess(payload.getId(), payload.getUsername(), current->getTenantId()))
            {
                throw DrugServiceException(DrugServiceError::CrossTenant);
            }
            currentVersion = current->getVersion();
            if (item->version.getValue(-1) != current->getVersion())
            {
                failureCode = MedicineV2BusinessCode::VERSION_CONFLICT;
                failureMessage = ZH_WORDS_GETTER("drug.v2.message.versionConflict");
                throw DrugServiceException(DrugServiceError::ConcurrentModification);
            }
            if (current->getStatusEnum() != expectedStatus || current->getActiveFlag() == enable)
            {
                throw DrugServiceException(DrugServiceError::InvalidStatus);
            }
            const std::list<std::string> ids = {id};
            if (enable)
            {
                const auto readiness = dao.selectEnableReadiness(ids, current->getTenantId());
                if (!readiness || !readiness->satisfied)
                {
                    throw DrugServiceException(DrugServiceError::InvalidInput);
                }
            }

            const std::string operationTime = SimpleDateTimeFormat::format();
            if (dao.updateActiveStatusByVersion(id, current->getTenantId(), current->getVersion(), expectedStatus,
                                                targetStatus, enable, auditOperator, operationTime) != 1)
            {
                failureCode = MedicineV2BusinessCode::VERSION_CONFLICT;
                failureMessage = ZH_WORDS_GETTER("drug.v2.message.versionConflict");
                throw DrugServiceException(DrugServiceError::ConcurrentModification);
            }
            transaction.commit();
            result->successItems->push_back(makeMedicineVersionItem(id, current->getVersion() + 1));
        }
        catch (const DrugServiceException &error)
        {
            failureCode = medicineV2FailureCode(error.getError());
            failureMessage = medicineV2FailureMessage(error.getError());
            try
            {
                dao.getSqlSession()->rollbackTransaction();
            }
            catch (...)
            {
            }
            dao.releaseSqlSession();
            addMedicineV2Failure(result, item->id.getValue(""), failureCode, failureMessage,
                                 currentVersion);
        }
        catch (...)
        {
            try
            {
                dao.getSqlSession()->rollbackTransaction();
            }
            catch (...)
            {
            }
            dao.releaseSqlSession();
            addMedicineV2Failure(result, item->id.getValue(""), MedicineV2BusinessCode::SERVER_ERROR,
                                 ZH_WORDS_GETTER("drug.v2.message.serverError"), currentVersion);
        }
    }
    return result;
}

MedicineBatchResultDTO::Wrapper DrugService::batchDeleteMedicinesV2(
    const MedicineBatchDeleteDTO::Wrapper &dto)
{
    if (!dto || dto->getPayload() == nullptr)
    {
        throw DrugServiceException(DrugServiceError::InvalidInput);
    }
    const PayloadDTO &payload = *dto->getPayload();
    const std::string auditOperator = operatorId(payload);
    auto result = MedicineBatchResultDTO::createEmpty();

    // 每个药品独立检查引用并提交软删除。
    for (const auto &item : *dto->items.get())
    {
        DrugDAO dao;
        oatpp::Int32 currentVersion = nullptr;
        int failureCode = MedicineV2BusinessCode::BUSINESS_VALIDATION_FAILED;
        std::string failureMessage = ZH_WORDS_GETTER("drug.v2.message.businessFailed");
        try
        {
            TransactionManager transaction(&dao);
            const std::string id = item->id.getValue("");
            const auto current = dao.selectById(id, true);
            if (!current)
            {
                throw DrugServiceException(DrugServiceError::NotFound);
            }
            if (!dao.hasTenantAccess(payload.getId(), payload.getUsername(), current->getTenantId()))
            {
                throw DrugServiceException(DrugServiceError::CrossTenant);
            }
            currentVersion = current->getVersion();
            if (item->version.getValue(-1) != current->getVersion())
            {
                failureCode = MedicineV2BusinessCode::VERSION_CONFLICT;
                failureMessage = ZH_WORDS_GETTER("drug.v2.message.versionConflict");
                throw DrugServiceException(DrugServiceError::ConcurrentModification);
            }
            if (current->getStatusEnum() != DrugConstants::STATUS_INACTIVE || current->getActiveFlag())
            {
                throw DrugServiceException(DrugServiceError::InvalidStatus);
            }

            const std::list<std::string> ids = {id};
            const auto referenceCheck = dao.selectReferenceCheck(ids);
            if (!referenceCheck || referenceCheck->referenced)
            {
                throw DrugServiceException(referenceCheck ? DrugServiceError::Referenced
                                                          : DrugServiceError::DatabaseFailure);
            }

            const std::string operationTime = SimpleDateTimeFormat::format();
            dao.softDeleteAllPrices(ids, current->getTenantId(), auditOperator, operationTime);
            const auto activePrices = dao.selectActivePricePresence(ids, current->getTenantId());
            if (!activePrices || activePrices->satisfied)
            {
                throw DrugServiceException(DrugServiceError::DatabaseFailure);
            }
            if (dao.softDeleteDrugByVersion(id, current->getTenantId(), current->getVersion(), auditOperator,
                                            operationTime) != 1)
            {
                failureCode = MedicineV2BusinessCode::VERSION_CONFLICT;
                failureMessage = ZH_WORDS_GETTER("drug.v2.message.versionConflict");
                throw DrugServiceException(DrugServiceError::ConcurrentModification);
            }
            transaction.commit();
            result->successItems->push_back(makeMedicineVersionItem(id, current->getVersion() + 1));
        }
        catch (const DrugServiceException &error)
        {
            failureCode = medicineV2FailureCode(error.getError());
            failureMessage = medicineV2FailureMessage(error.getError());
            try
            {
                dao.getSqlSession()->rollbackTransaction();
            }
            catch (...)
            {
            }
            dao.releaseSqlSession();
            addMedicineV2Failure(result, item->id.getValue(""), failureCode, failureMessage,
                                 currentVersion);
        }
        catch (...)
        {
            try
            {
                dao.getSqlSession()->rollbackTransaction();
            }
            catch (...)
            {
            }
            dao.releaseSqlSession();
            addMedicineV2Failure(result, item->id.getValue(""), MedicineV2BusinessCode::SERVER_ERROR,
                                 ZH_WORDS_GETTER("drug.v2.message.serverError"), currentVersion);
        }
    }
    return result;
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
