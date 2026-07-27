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
#ifndef _chinapres3MedicationDispenseDO_H_
#define _chinapres3MedicationDispenseDO_H_

#include "../DoInclude.h"

/**
 * 中药处方发药记录数据库映射DO
 *
 * 对应数据库表 med_medication_dispense
 * 存储中药处方开立后生成的发药单据信息
 *
 * 宏定义说明
 *   - 宏 MYSQL_SYNTHESIZE 用于自动生成数据库字段的get/set访问方法
 *     声明数据库实体对应的成员变量
 *   - 区分核心字段与扩展字段：核心字段为新增时必填字段
 *     创建人、创建时间等扩展字段在执行插入时自动赋值填充
 *
 * @see chinapres3MedicationRequestDO 处方主表，通过 medReqId 关联处方单据
 */
class chinapres3MedicationDispenseDO : public BaseDO
{
	// =============================
	// 主键字段
	// =============================
	// 记录唯一主键标识
	MYSQL_SYNTHESIZE(string, id, Id);

	// =============================
	// DML核心字段，新增操作必填
	// =============================
	// 处方单据ID，关联中药处方主表 med_medication_request.id
	MYSQL_SYNTHESIZE(string, medReqId, MedReqId);
	// 患者ID，关联患者表 adm_patient.id
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// 就诊ID，关联就诊记录表 adm_encounter.id
	MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
	// 发药业务流水号，全局唯一标识
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// 发药状态枚举：PREPARATION 待调配 / DISPENSED 已发药
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

	// =============================
	// 扩展辅助字段
	// =============================
	// 发药场景枚举，区分门诊/住院发药类型
	MYSQL_SYNTHESIZE(string, dispenseEnum, DispenseEnum);
	// 药房库房ID，关联库房表 adm_location.id
	MYSQL_SYNTHESIZE(string, locationId, LocationId);
	// 记录创建操作人账号
	MYSQL_SYNTHESIZE(string, createBy, CreateBy);
	// 记录创建时间
	MYSQL_SYNTHESIZE(string, createAt, CreateAt);
	// 记录更新操作人账号
	MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
	// 记录更新时间
	MYSQL_SYNTHESIZE(string, updateAt, UpdateAt);

public:
	chinapres3MedicationDispenseDO() : BaseDO("med_medication_dispense")
	{
		// 主键
		MYSQL_ADD_FIELD_PK("id", "s", id);

		// DML核心字段，新增操作必填
		MYSQL_ADD_FIELD("med_req_id", "s", medReqId);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
	}
};

// 智能指针别名，业务Service与DAO统一使用
typedef std::shared_ptr<chinapres3MedicationDispenseDO> Ptrchinapres3MedicationDispenseDO;

#endif // !_chinapres3MedicationDispenseDO_H_