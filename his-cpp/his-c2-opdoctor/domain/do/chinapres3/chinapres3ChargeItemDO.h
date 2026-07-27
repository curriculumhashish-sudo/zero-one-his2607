#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2025/07/26

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
#ifndef _chinapres3ChargeItemDO_H_
#define _chinapres3ChargeItemDO_H_

#include "../DoInclude.h"

/**
 * 中药处方收费项目数据库映射DO
 *
 * 对应数据库表 adm_charge_item
 * 存储门诊中药处方开立后生成的收费明细记录
 *
 * 宏定义说明
 *   - 宏 MYSQL_SYNTHESIZE 用于自动生成数据库字段的get/set访问方法
 *     声明数据库实体对应的成员变量
 *   * 标记字段区分：业务核心字段（INSERT、查询条件必带）与扩展字段
 *     构造函数仅注册核心字段，扩展字段动态拼接SQL使用
 *
 * @see chinapres3MedicationRequestDO 处方主表，通过 serviceTable + serviceId 关联收费记录
 */
class chinapres3ChargeItemDO : public BaseDO
{
	// =============================
	// 主键字段
	// =============================
	// 记录唯一主键标识
	MYSQL_SYNTHESIZE(string, id, Id);

	// =============================
	// DML核心字段，新增/查询条件必填
	// =============================
	// 关联业务表名，固定为 "med_medication_request" 代表中药处方单据
	MYSQL_SYNTHESIZE(string, serviceTable, ServiceTable);
	// 关联业务单据ID，对应serviceTable表的主键
	MYSQL_SYNTHESIZE(string, serviceId, ServiceId);
	// 患者ID，关联表 adm_patient.id
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// 就诊ID，关联表 adm_encounter.id
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// 处方编号，中药处方单据唯一流水号
	MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);

	// =============================
	// 收费金额、扩展关联字段
	// =============================
	// 收费项目定义ID，关联收费项目定义表 adm_charge_item_definition
	MYSQL_SYNTHESIZE(string, definitionId, DefinitionId);
	// 项目总金额
	MYSQL_SYNTHESIZE(double, totalPrice, TotalPrice);
	// 项目数量数值
	MYSQL_SYNTHESIZE(double, quantityValue, QuantityValue);
	// 数量单位（剂、盒、付等）
	MYSQL_SYNTHESIZE(string, quantityUnit, QuantityUnit);
	// 项目单价
	MYSQL_SYNTHESIZE(double, unitPrice, UnitPrice);
	// 结算账户ID
	MYSQL_SYNTHESIZE(string, accountId, AccountId);
	// 机构ID，院区/科室标识
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	// 诊断分类ID，关联诊断定义表 cln_condition_definition
	MYSQL_SYNTHESIZE(string, conditionId, ConditionId);
	// 就诊诊断ID，关联就诊诊断记录表 adm_encounter_diagnosis
	MYSQL_SYNTHESIZE(string, encounterDiagnosisId, EncounterDiagnosisId);
	// 收费来源枚举，区分开立场景（TCM_ISSUE代表中药处方发药）
	MYSQL_SYNTHESIZE(string, generateSourceEnum, GenerateSourceEnum);
	// 就诊场景枚举，OUTPATIENT门诊/INPATIENT住院
	MYSQL_SYNTHESIZE(string, contextEnum, ContextEnum);
	// 创建人账号
	MYSQL_SYNTHESIZE(string, createBy, CreateBy);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createAt, CreateAt);
	// 更新人账号
	MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
	// 更新时间
	MYSQL_SYNTHESIZE(string, updateAt, UpdateAt);

public:
	chinapres3ChargeItemDO() : BaseDO("adm_charge_item")
	{
		// 主键
		MYSQL_ADD_FIELD_PK("id", "s", id);

		// DML核心字段，新增/查询条件必填
		MYSQL_ADD_FIELD("service_table", "s", serviceTable);
		MYSQL_ADD_FIELD("service_id", "s", serviceId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
	}
};

// 智能指针别名，业务Service与DAO统一使用
typedef std::shared_ptr<chinapres3ChargeItemDO> Ptrchinapres3ChargeItemDO;

#endif // !_chinapres3ChargeItemDO_H_