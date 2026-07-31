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
 * 诊疗主表字段映射（列表等返回值对象）
 * 对应表：wkf_activity_definition
 */
class WkfActivityDefinitionMapper : public Mapper<TreatmentDO>
{
public:
	TreatmentDO mapper(ResultSet* resultSet) const override
	{
		TreatmentDO data;
		data.setId(resultSet->getString("id"));
		data.setTenantId(resultSet->getString("tenant_id"));
		data.setCreatedAt(resultSet->getString("created_at"));
		data.setUpdatedAt(resultSet->getString("updated_at"));
		data.setCreatedBy(resultSet->getString("created_by"));
		data.setUpdatedBy(resultSet->getString("updated_by"));
		data.setIsDeleted(resultSet->getInt("is_deleted"));
		data.setDeletedAt(resultSet->getString("deleted_at"));
		data.setDeletedBy(resultSet->getString("deleted_by"));
		data.setVersion(resultSet->getInt("version"));

		data.setCategoryCode(resultSet->getString("category_code"));
		data.setBusNo(resultSet->getString("bus_no"));
		data.setName(resultSet->getString("name"));
		data.setPyStr(resultSet->getString("py_str"));
		data.setWbStr(resultSet->getString("wb_str"));
		data.setPackageUnitCode(resultSet->getString("package_unit_code"));
		data.setTypeEnum(resultSet->getString("type_enum"));
		data.setStatusEnum(resultSet->getString("status_enum"));
		data.setDescriptionText(resultSet->getString("description_text"));
		data.setBodySiteCode(resultSet->getString("body_site_code"));
		data.setSpecimenCode(resultSet->getString("specimen_code"));
		data.setOrgId(resultSet->getString("org_id"));
		data.setLocationId(resultSet->getString("location_id"));
		data.setRuleId(resultSet->getInt("rule_id"));
		data.setPricingFlag(resultSet->getInt("pricing_flag"));
		data.setYbFlag(resultSet->getInt("yb_flag"));
		data.setYbNo(resultSet->getString("yb_no"));
		data.setYbMatchFlag(resultSet->getInt("yb_match_flag"));
		data.setChrgitmLv(resultSet->getInt("chrgitm_lv"));
		data.setChildrenJson(resultSet->getString("children_json"));
		data.setChildrenFlag(resultSet->getInt("children_flag"));
		data.setActivityVersion(resultSet->getString("activity_version"));
		data.setMerchandiseName(resultSet->getString("merchandise_name"));
		data.setMerchandisePyStr(resultSet->getString("merchandise_py_str"));
		data.setMerchandiseWbStr(resultSet->getString("merchandise_wb_str"));
		data.setDefEncounterUnitCode(resultSet->getString("def_encounter_unit_code"));
		data.setDefInhospitalUnitCode(resultSet->getString("def_inhospital_unit_code"));
		return data;
	}
};

/**
 * 诊疗主表字段映射（详情等返回智能指针，无数据时为 nullptr）
 * 对应表：wkf_activity_definition
 */
class PtrWkfActivityDefinitionMapper : public Mapper<PtrTreatmentDO>
{
public:
	PtrTreatmentDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<TreatmentDO>();
		data->setId(resultSet->getString("id"));
		data->setTenantId(resultSet->getString("tenant_id"));
		data->setCreatedAt(resultSet->getString("created_at"));
		data->setUpdatedAt(resultSet->getString("updated_at"));
		data->setCreatedBy(resultSet->getString("created_by"));
		data->setUpdatedBy(resultSet->getString("updated_by"));
		data->setIsDeleted(resultSet->getInt("is_deleted"));
		data->setDeletedAt(resultSet->getString("deleted_at"));
		data->setDeletedBy(resultSet->getString("deleted_by"));
		data->setVersion(resultSet->getInt("version"));

		data->setCategoryCode(resultSet->getString("category_code"));
		data->setBusNo(resultSet->getString("bus_no"));
		data->setName(resultSet->getString("name"));
		data->setPyStr(resultSet->getString("py_str"));
		data->setWbStr(resultSet->getString("wb_str"));
		data->setPackageUnitCode(resultSet->getString("package_unit_code"));
		data->setTypeEnum(resultSet->getString("type_enum"));
		data->setStatusEnum(resultSet->getString("status_enum"));
		data->setDescriptionText(resultSet->getString("description_text"));
		data->setBodySiteCode(resultSet->getString("body_site_code"));
		data->setSpecimenCode(resultSet->getString("specimen_code"));
		data->setOrgId(resultSet->getString("org_id"));
		data->setLocationId(resultSet->getString("location_id"));
		data->setRuleId(resultSet->getInt("rule_id"));
		data->setPricingFlag(resultSet->getInt("pricing_flag"));
		data->setYbFlag(resultSet->getInt("yb_flag"));
		data->setYbNo(resultSet->getString("yb_no"));
		data->setYbMatchFlag(resultSet->getInt("yb_match_flag"));
		data->setChrgitmLv(resultSet->getInt("chrgitm_lv"));
		data->setChildrenJson(resultSet->getString("children_json"));
		data->setChildrenFlag(resultSet->getInt("children_flag"));
		data->setActivityVersion(resultSet->getString("activity_version"));
		data->setMerchandiseName(resultSet->getString("merchandise_name"));
		data->setMerchandisePyStr(resultSet->getString("merchandise_py_str"));
		data->setMerchandiseWbStr(resultSet->getString("merchandise_wb_str"));
		data->setDefEncounterUnitCode(resultSet->getString("def_encounter_unit_code"));
		data->setDefInhospitalUnitCode(resultSet->getString("def_inhospital_unit_code"));
		return data;
	}
};

#endif // !_WKF_ACTIVITY_DEFINITION_MAPPER_H_
