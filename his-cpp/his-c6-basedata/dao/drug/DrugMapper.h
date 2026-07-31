#pragma once
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
#ifndef _DRUG_MAPPER_H_
#define _DRUG_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/drug/MedMedicationDefinitionDO.h"
#include "../../domain/do/adm/AdmChargeItemDefinitionDO.h"

/**
 * Maps the explicit full-column drug query.
 */
class PtrDrugMapper : public Mapper<PtrMedMedicationDefinitionDO>
{
  public:
    PtrMedMedicationDefinitionDO mapper(ResultSet *resultSet) const override
    {
        auto data = std::make_shared<MedMedicationDefinitionDO>();
        data->setId(resultSet->getString(1));
        data->setTenantId(resultSet->getString(2));
        data->setCreatedAt(resultSet->getString(3));
        data->setUpdatedAt(resultSet->getString(4));
        data->setCreatedBy(resultSet->getString(5));
        data->setUpdatedBy(resultSet->getString(6));
        data->setIsDeleted(resultSet->getBoolean(7));
        data->setDeletedAt(resultSet->getString(8));
        data->setDeletedBy(resultSet->getString(9));
        data->setVersion(resultSet->getInt(10));
        data->setBusNo(resultSet->getString(11));
        data->setName(resultSet->getString(12));
        data->setDomainEnum(resultSet->getString(13));
        data->setMedVersion(resultSet->getString(14));
        data->setNameEn(resultSet->getString(15));
        data->setPyStr(resultSet->getString(16));
        data->setWbStr(resultSet->getString(17));
        data->setCategoryCode(resultSet->getString(18));
        data->setMerchandiseName(resultSet->getString(19));
        data->setMerchandisePyStr(resultSet->getString(20));
        data->setMerchandiseWbStr(resultSet->getString(21));
        data->setPurchaseUnitCode(resultSet->getString(22));
        data->setSplitUnitCode(resultSet->getString(23));
        data->setComprisedText(resultSet->getString(24));
        data->setIngredient(resultSet->getString(25));
        data->setDoseFrom(resultSet->getInt(26));
        data->setApprovalNumber(resultSet->getString(27));
        data->setYbMatchFlag(resultSet->getBoolean(28));
        data->setYbNo(resultSet->getString(29));
        data->setPharmacologyCategoryCode(resultSet->getString(30));
        data->setSkinTestFlag(resultSet->getBoolean(31));
        data->setInjectFlag(resultSet->getBoolean(32));
        data->setManufacturerId(resultSet->getString(33));
        data->setManufacturerText(resultSet->getString(34));
        data->setSupplyId(resultSet->getString(35));
        data->setRestrictedFlag(resultSet->getBoolean(36));
        data->setRestrictedScope(resultSet->getString(37));
        data->setActiveFlag(resultSet->getBoolean(38));
        data->setChildrenFlag(resultSet->getBoolean(39));
        data->setCharacteristic(resultSet->getInt(40));
        data->setItemMinQuantity(resultSet->getDouble(41));
        data->setItemMaxQuantity(resultSet->getDouble(42));
        data->setDefEncounterUnitCode(resultSet->getString(43));
        data->setDefInhospitalUnitCode(resultSet->getString(44));
        data->setNationalDrugCode(resultSet->getString(45));
        data->setPartAttributeEnum(resultSet->getString(46));
        data->setAntibioticCode(resultSet->getString(47));
        data->setRestrictedEnum(resultSet->getString(48));
        data->setSelfFlag(resultSet->getBoolean(49));
        data->setAntibioticFlag(resultSet->getBoolean(50));
        data->setBasicFlag(resultSet->getBoolean(51));
        data->setThoPartAttributeEnum(resultSet->getString(52));
        data->setChrgitmLv(resultSet->getInt(53));
        data->setRxFlag(resultSet->getBoolean(54));
        data->setDosageInstruction(resultSet->getString(55));
        data->setDrug69Code(resultSet->getString(56));
        data->setTotalVolume(resultSet->getString(57));
        data->setDoseFormCode(resultSet->getString(58));
        data->setDoseUnitCode(resultSet->getString(59));
        data->setPackageUnitCode(resultSet->getString(60));
        data->setStockUnitCode(resultSet->getString(61));
        data->setMethodCode(resultSet->getString(62));
        data->setRateCode(resultSet->getString(63));
        data->setDose(resultSet->getDouble(64));
        data->setMaxUnit(resultSet->getDouble(65));
        data->setUsageLimit(resultSet->getDouble(66));
        data->setLocationId(resultSet->getString(67));
        data->setStatusEnum(resultSet->getString(68));
        data->setOrgId(resultSet->getString(69));
        data->setIngredientItem(resultSet->getString(70));
        data->setMedicationDefinition(resultSet->getString(71));
        data->setDddCode(resultSet->getString(72));
        data->setDddUnitCode(resultSet->getString(73));
        data->setMinRateCode(resultSet->getString(74));
        data->setMaxRateCode(resultSet->getString(75));
        data->setDoseUnitRatio(resultSet->getDouble(76));
        data->setPurchaseUnitRatio(resultSet->getDouble(77));
        data->setStockUnitRatio(resultSet->getDouble(78));
        data->setPackageUnitRatio(resultSet->getDouble(79));
        return data;
    }
};

/**
 * Maps only the columns needed for batch state transitions.
 */
class PtrDrugStateMapper : public Mapper<PtrMedMedicationDefinitionDO>
{
  public:
    PtrMedMedicationDefinitionDO mapper(ResultSet *resultSet) const override
    {
        auto data = std::make_shared<MedMedicationDefinitionDO>();
        data->setId(resultSet->getString(1));
        data->setTenantId(resultSet->getString(2));
        data->setIsDeleted(resultSet->getBoolean(3));
        data->setVersion(resultSet->getInt(4));
        data->setStatusEnum(resultSet->getString(5));
        data->setActiveFlag(resultSet->getBoolean(6));
        return data;
    }
};

/**
 * Maps active base-price rows.
 */
class PtrDrugPriceMapper : public Mapper<PtrAdmChargeItemDefinitionDO>
{
  public:
    PtrAdmChargeItemDefinitionDO mapper(ResultSet *resultSet) const override
    {
        auto data = std::make_shared<AdmChargeItemDefinitionDO>();
        data->setPriceTypeCode(resultSet->getString(1));
        data->setPriceValue(resultSet->getDouble(2));
        data->setTypeCode(resultSet->getString(3));
        data->setYbType(resultSet->getString(4));
        return data;
    }
};

struct DrugModifyTextResult
{
    std::string purchaseUnitName;
    std::string splitUnitName;
    std::string doseUnitName;
    std::string packageUnitName;
    std::string stockUnitName;
    std::string doseFromName;
    std::string antibioticName;
    std::string typeName;
    std::string categoryName;
    std::string locationName;
    std::string supplierName;
};

using PtrDrugModifyTextResult = std::shared_ptr<DrugModifyTextResult>;

/**
 * Maps display text needed by the drug modification response.
 */
class PtrDrugModifyTextMapper : public Mapper<PtrDrugModifyTextResult>
{
  public:
    PtrDrugModifyTextResult mapper(ResultSet *resultSet) const override
    {
        auto data = std::make_shared<DrugModifyTextResult>();
        data->purchaseUnitName = resultSet->getString(1);
        data->splitUnitName = resultSet->getString(2);
        data->doseUnitName = resultSet->getString(3);
        data->packageUnitName = resultSet->getString(4);
        data->stockUnitName = resultSet->getString(5);
        data->doseFromName = resultSet->getString(6);
        data->antibioticName = resultSet->getString(7);
        data->typeName = resultSet->getString(8);
        data->categoryName = resultSet->getString(9);
        data->locationName = resultSet->getString(10);
        data->supplierName = resultSet->getString(11);
        return data;
    }
};

struct DrugReferenceCheck
{
    bool referenced = false;
};

using PtrDrugReferenceCheck = std::shared_ptr<DrugReferenceCheck>;

/**
 * Keeps an empty result distinguishable from a database query failure.
 */
class PtrDrugReferenceCheckMapper : public Mapper<PtrDrugReferenceCheck>
{
  public:
    PtrDrugReferenceCheck mapper(ResultSet *resultSet) const override
    {
        auto result = std::make_shared<DrugReferenceCheck>();
        result->referenced = resultSet->getBoolean(1);
        return result;
    }
};

struct DrugConditionCheck
{
    bool satisfied = false;
};

using PtrDrugConditionCheck = std::shared_ptr<DrugConditionCheck>;

/**
 * Maps a single boolean business-condition result.
 */
class PtrDrugConditionCheckMapper : public Mapper<PtrDrugConditionCheck>
{
  public:
    PtrDrugConditionCheck mapper(ResultSet *resultSet) const override
    {
        auto result = std::make_shared<DrugConditionCheck>();
        result->satisfied = resultSet->getBoolean(1);
        return result;
    }
};

#endif // !_DRUG_MAPPER_H_
