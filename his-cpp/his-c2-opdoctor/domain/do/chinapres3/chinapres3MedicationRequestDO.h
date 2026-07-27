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
#ifndef _chinapres3MedicationRequestDO_H_
#define _chinapres3MedicationRequestDO_H_

#include "../DoInclude.h"

/**
 * 处方单据数据对象
 *
 * 数据库表：med_medication_request
 *
 * 字段注册策略：
 *   - 所有字段通过 MYSQL_SYNTHESIZE 宏声明，用于查询结果集映射。
 *   - 仅数据库操作核心字段在构造函数内注册
 *     （供更新(UPDATE)操作使用）。
 */
class chinapres3MedicationRequestDO : public BaseDO
{
	// =============================
	// 主键字段
	// =============================
	MYSQL_SYNTHESIZE(string, id, Id);

	// =============================
	// 数据库操作核心字段（更新操作必填）
	// =============================
	// 处方编号，同一张处方下多条药品记录共用同一个处方号
	MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
	// 患者ID，关联患者表 adm_patient.id
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// 就诊ID，关联就诊记录表 adm_encounter.id
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// 处方状态枚举（草稿DRAFT、已开立ACTIVE、已作废CANCELLED等）
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

	// =============================
	// 自动填充/非核心字段
	// =============================
	// 处方类型编码
	MYSQL_SYNTHESIZE(string, rxTypeCode, RxTypeCode);
	// 创建人账号
	MYSQL_SYNTHESIZE(string, createBy, CreateBy);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createAt, CreateAt);
	// 更新人账号
	MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
	// 更新时间
	MYSQL_SYNTHESIZE(string, updateAt, UpdateAt);

public:
	chinapres3MedicationRequestDO() : BaseDO("med_medication_request")
	{
		// 主键
		MYSQL_ADD_FIELD_PK("id", "s", id);

		// 数据库操作核心字段（更新操作必填）
		MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
	}
};

// 智能指针类型别名
typedef std::shared_ptr<chinapres3MedicationRequestDO> Ptrchinapres3MedicationRequestDO;

#endif // !_chinapres3MedicationRequestDO_H_