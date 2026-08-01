#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Date: 2026/07/25 11:20:00

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
#ifndef _WKF_ACTIVITY_DEFINITION_MAPPER_H_
#define _WKF_ACTIVITY_DEFINITION_MAPPER_H_

#include "Mapper.h"
#include "../../domain/do/treatment/WkfActivityDefinitionDo.h"

/**
 * Main table mapper for wkf_activity_definition (1-based column index).
 * Matches TREATMENT_FULL_COLUMNS (37 columns, no price column).
 * Prices are loaded separately via adm_charge_item_definition.
 *
 * 1 id ... 10 version, 11 category_code ... 37 def_inhospital_unit_code
 */
class WkfActivityDefinitionMapper : public Mapper<TreatmentDO>
{
public:
	TreatmentDO mapper(ResultSet* resultSet) const override
	{
		TreatmentDO data;
		data.setId(resultSet->getString(1));
		data.setTenantId(resultSet->getString(2));
		data.setCreatedAt(resultSet->getString(3));
		data.setUpdatedAt(resultSet->getString(4));
		data.setCreatedBy(resultSet->getString(5));
		data.setUpdatedBy(resultSet->getString(6));
		data.setIsDeleted(resultSet->getInt(7));
		data.setDeletedAt(resultSet->getString(8));
		data.setDeletedBy(resultSet->getString(9));
		data.setVersion(resultSet->getInt(10));

		data.setCategoryCode(resultSet->getString(11));
		data.setBusNo(resultSet->getString(12));
		data.setName(resultSet->getString(13));
		data.setPyStr(resultSet->getString(14));
		data.setWbStr(resultSet->getString(15));
		data.setPackageUnitCode(resultSet->getString(16));
		data.setTypeEnum(resultSet->getString(17));
		data.setStatusEnum(resultSet->getString(18));
		data.setDescriptionText(resultSet->getString(19));
		data.setBodySiteCode(resultSet->getString(20));
		data.setSpecimenCode(resultSet->getString(21));
		data.setOrgId(resultSet->getString(22));
		data.setLocationId(resultSet->getString(23));
		data.setRuleId(resultSet->getInt(24));
		data.setPricingFlag(resultSet->getInt(25));
		data.setYbFlag(resultSet->getInt(26));
		data.setYbNo(resultSet->getString(27));
		data.setYbMatchFlag(resultSet->getInt(28));
		data.setChrgitmLv(resultSet->getInt(29));
		data.setChildrenJson(resultSet->getString(30));
		data.setChildrenFlag(resultSet->getInt(31));
		data.setActivityVersion(resultSet->getString(32));
		data.setMerchandiseName(resultSet->getString(33));
		data.setMerchandisePyStr(resultSet->getString(34));
		data.setMerchandiseWbStr(resultSet->getString(35));
		data.setDefEncounterUnitCode(resultSet->getString(36));
		data.setDefInhospitalUnitCode(resultSet->getString(37));
		// No column 38 here; retail price comes from charge table (fillPrices).
		return data;
	}
};

/**
 * Pointer mapper for detail query (nullptr when empty).
 * Same 37-column layout as WkfActivityDefinitionMapper.
 */
class PtrWkfActivityDefinitionMapper : public Mapper<PtrTreatmentDO>
{
public:
	PtrTreatmentDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<TreatmentDO>();
		data->setId(resultSet->getString(1));
		data->setTenantId(resultSet->getString(2));
		data->setCreatedAt(resultSet->getString(3));
		data->setUpdatedAt(resultSet->getString(4));
		data->setCreatedBy(resultSet->getString(5));
		data->setUpdatedBy(resultSet->getString(6));
		data->setIsDeleted(resultSet->getInt(7));
		data->setDeletedAt(resultSet->getString(8));
		data->setDeletedBy(resultSet->getString(9));
		data->setVersion(resultSet->getInt(10));

		data->setCategoryCode(resultSet->getString(11));
		data->setBusNo(resultSet->getString(12));
		data->setName(resultSet->getString(13));
		data->setPyStr(resultSet->getString(14));
		data->setWbStr(resultSet->getString(15));
		data->setPackageUnitCode(resultSet->getString(16));
		data->setTypeEnum(resultSet->getString(17));
		data->setStatusEnum(resultSet->getString(18));
		data->setDescriptionText(resultSet->getString(19));
		data->setBodySiteCode(resultSet->getString(20));
		data->setSpecimenCode(resultSet->getString(21));
		data->setOrgId(resultSet->getString(22));
		data->setLocationId(resultSet->getString(23));
		data->setRuleId(resultSet->getInt(24));
		data->setPricingFlag(resultSet->getInt(25));
		data->setYbFlag(resultSet->getInt(26));
		data->setYbNo(resultSet->getString(27));
		data->setYbMatchFlag(resultSet->getInt(28));
		data->setChrgitmLv(resultSet->getInt(29));
		data->setChildrenJson(resultSet->getString(30));
		data->setChildrenFlag(resultSet->getInt(31));
		data->setActivityVersion(resultSet->getString(32));
		data->setMerchandiseName(resultSet->getString(33));
		data->setMerchandisePyStr(resultSet->getString(34));
		data->setMerchandiseWbStr(resultSet->getString(35));
		data->setDefEncounterUnitCode(resultSet->getString(36));
		data->setDefInhospitalUnitCode(resultSet->getString(37));
		// No column 38 here; retail price comes from charge table (fillPrices).
		return data;
	}
};

#endif // !_WKF_ACTIVITY_DEFINITION_MAPPER_H_
