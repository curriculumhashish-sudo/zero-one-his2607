#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Date: 2026/07/24 18:00:00

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
#ifndef _TREATMENTDO_H_
#define _TREATMENTDO_H_

#include "../DoInclude.h"

/**
 * 诊疗主数据 DO，对应表 wkf_activity_definition
 * 枚举类字段（type_enum / status_enum）按方案 A 使用 string 存储
 */
class TreatmentDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	// 软删标记
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	// 乐观锁版本号
	MYSQL_SYNTHESIZE(int, version, Version);

	// 目录分类编码
	MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
	// 业务编码（对应 DTO 的 code）
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// 诊疗名称
	MYSQL_SYNTHESIZE(string, name, Name);
	// 拼音检索码
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	// 五笔检索码
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	// 销售/计价单位编码
	MYSQL_SYNTHESIZE(string, packageUnitCode, PackageUnitCode);
	// 业务类型枚举（字符串，如 "5"）
	MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
	// 状态枚举（字符串，如 "ACTIVE"）
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	// 描述文本
	MYSQL_SYNTHESIZE(string, descriptionText, DescriptionText);
	// 身体部位编码
	MYSQL_SYNTHESIZE(string, bodySiteCode, BodySiteCode);
	// 标本编码
	MYSQL_SYNTHESIZE(string, specimenCode, SpecimenCode);
	// 所属机构标识
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	// 位置标识
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	// 计费规则标识
	MYSQL_SYNTHESIZE(int, ruleId, RuleId);
	// 划价标记
	MYSQL_SYNTHESIZE(int, pricingFlag, PricingFlag);
	// 医保标记
	MYSQL_SYNTHESIZE(int, ybFlag, YbFlag);
	// 医保编号
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	// 医保对码标记
	MYSQL_SYNTHESIZE(int, ybMatchFlag, YbMatchFlag);
	// 医保收费等级
	MYSQL_SYNTHESIZE(int, chrgitmLv, ChrgitmLv);
	// 子项 JSON 快照
	MYSQL_SYNTHESIZE(string, childrenJson, ChildrenJson);
	// 子项标记
	MYSQL_SYNTHESIZE(int, childrenFlag, ChildrenFlag);
	// 诊疗业务版本（非乐观锁 version）
	MYSQL_SYNTHESIZE(string, activityVersion, ActivityVersion);
	// 商品名称
	MYSQL_SYNTHESIZE(string, merchandiseName, MerchandiseName);
	// 商品名拼音检索码
	MYSQL_SYNTHESIZE(string, merchandisePyStr, MerchandisePyStr);
	// 商品名五笔检索码
	MYSQL_SYNTHESIZE(string, merchandiseWbStr, MerchandiseWbStr);
	// 默认门诊单位编码
	MYSQL_SYNTHESIZE(string, defEncounterUnitCode, DefEncounterUnitCode);
	// 默认住院单位编码
	MYSQL_SYNTHESIZE(string, defInhospitalUnitCode, DefInhospitalUnitCode);

public:
	TreatmentDO() : BaseDO("wkf_activity_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

		MYSQL_ADD_FIELD_NULLABLE("category_code", "s", categoryCode, false);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("package_unit_code", "s", packageUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("type_enum", "s", typeEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("description_text", "s", descriptionText, true);
		MYSQL_ADD_FIELD_NULLABLE("body_site_code", "s", bodySiteCode, true);
		MYSQL_ADD_FIELD_NULLABLE("specimen_code", "s", specimenCode, true);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("location_id", "s", locationId, true);
		MYSQL_ADD_FIELD_NULLABLE("rule_id", "i", ruleId, true);
		MYSQL_ADD_FIELD_NULLABLE("pricing_flag", "i", pricingFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_flag", "i", ybFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_match_flag", "i", ybMatchFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("chrgitm_lv", "i", chrgitmLv, true);
		MYSQL_ADD_FIELD_NULLABLE("children_json", "s", childrenJson, true);
		MYSQL_ADD_FIELD_NULLABLE("children_flag", "i", childrenFlag, true);
		MYSQL_ADD_FIELD_NULLABLE("activity_version", "s", activityVersion, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_name", "s", merchandiseName, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_py_str", "s", merchandisePyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("merchandise_wb_str", "s", merchandiseWbStr, true);
		MYSQL_ADD_FIELD_NULLABLE("def_encounter_unit_code", "s", defEncounterUnitCode, true);
		MYSQL_ADD_FIELD_NULLABLE("def_inhospital_unit_code", "s", defInhospitalUnitCode, true);
	}
};

typedef std::shared_ptr<TreatmentDO> PtrTreatmentDO;

#endif // !_TREATMENTDO_H_
