#pragma once

#ifndef _MEDICATIONREQUESTDO_H_
#define _MEDICATIONREQUESTDO_H_

#include "../DoInclude.h"

/**
 * 药品申请DO
 * 对应数据库表：
 * med_medication_request
 * 说明：
 * 一张中医处方可能包含多种药材，
 * 每一种药材在该表中对应一条记录。
 * id:
 *      数据库记录唯一标识
 prescription_no:
      业务处方编号，同一张处方下多个药品记录共享
**/
class MedicationRequestDO : public BaseDO
{
	// =============================
	// 基础字段
	// =============================
	// 数据库记录唯一ID
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 最后更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 最后更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);

	// =============================
	// 处方关联信息
	// =============================
	// 处方编号
	// 一张处方对应多个药品记录
	MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
	// 患者ID
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	// 药品ID
	// 关联 med_medication_definition
	MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
	// 诊断定义ID
	// 关联 cln_condition_definition
	MYSQL_SYNTHESIZE(string, conditionDefinitionId, ConditionDefinitionId);

	// =============================
	// 用药信息
	// =============================
	// 药品数量
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	// 单次剂量
	// 例如：15g
	MYSQL_SYNTHESIZE(double, dose, Dose);
	// 剂量单位编码
	// 例如：g
	MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
	// 用法编码
	// 例如：煎服方式
	MYSQL_SYNTHESIZE(string, methodCode, MethodCode);
	// 频次编码
	// 例如：每日一次、每日两次
	MYSQL_SYNTHESIZE(string, rateCode, RateCode);
	// 用药说明
	// 医生填写的详细用法
	MYSQL_SYNTHESIZE(string, dosageInstruction, DosageInstruction);
	// 备注
	MYSQL_SYNTHESIZE(string, remark, Remark);

public:
	/**
	 * 构造函数
	 *
	 * 指定对应数据库表：
	 * med_medication_request
	 */
	MedicationRequestDO() : BaseDO("med_medication_request")
	{
		// =============================
		// 主键
		// =============================
		MYSQL_ADD_FIELD_PK("id", "s", id);

		// =============================
		// 基础字段
		// =============================
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);

		// =============================
		// 处方业务字段
		// =============================
		MYSQL_ADD_FIELD_NULLABLE("prescription_no", "s", prescriptionNo, true);//这个是处方id
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD("medication_id", "s", medicationId);
		MYSQL_ADD_FIELD_NULLABLE("condition_definition_id", "s", conditionDefinitionId, true);

		// =============================
		// 用药字段
		// =============================
		MYSQL_ADD_FIELD("quantity", "d", quantity);
		MYSQL_ADD_FIELD_NULLABLE("dose", "d", dose, true);
		MYSQL_ADD_FIELD_NULLABLE("dose_unit_code", "s", doseUnitCode, true);
		MYSQL_ADD_FIELD("method_code", "s", methodCode);
		MYSQL_ADD_FIELD("rate_code", "s", rateCode);
		MYSQL_ADD_FIELD_NULLABLE("dosage_instruction", "s", dosageInstruction, true);
		MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
	}
};

// 智能指针别名
typedef std::shared_ptr<MedicationRequestDO> PtrMedicationRequestDO;

#endif // !_MEDICATIONREQUESTDO_H_