#pragma once
#ifndef _ENTERORDERSDO_H_
#define _ENTERORDERSDO_H_
#include "../DoInclude.h"

//EnterOrders
//enter-orders
//ENTERORDERS

//获取历史病历列表（条件+分页）DO类
class EnterOrdersQueryDO : public BaseDO
{
	// 医嘱主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户id
	MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);
	// 诊断id
	MYSQL_SYNTHESIZE(string, encounter_id,Encounter_id);
	// 患者id
	MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
	// 小组id
	MYSQL_SYNTHESIZE(string, group_id, Group_id);
	// 医嘱状态
	MYSQL_SYNTHESIZE(string, status, Status);
	// 药品类型
	MYSQL_SYNTHESIZE(string, medicine_type, Medicine_type);
	// 医嘱类型
	MYSQL_SYNTHESIZE(string, therapy_type, Therapy_type);
	// 药品名称
	MYSQL_SYNTHESIZE(string, medicine_name, Medicine_name);
	// 药品规格
	MYSQL_SYNTHESIZE(string, specification, Specification);
	// 单次剂量
	MYSQL_SYNTHESIZE(string, dose, Dose);
	// 剂量单位
	MYSQL_SYNTHESIZE(string, dose_unit, Dose_unit);
	// 服用方式
	MYSQL_SYNTHESIZE(string, method, Method);
	// 服用时间
	MYSQL_SYNTHESIZE(string, rate, Rate);
	// 总金额
	MYSQL_SYNTHESIZE(string, total_price, Total_price);
	// 诊断描述
	MYSQL_SYNTHESIZE(string, description, Description);
	// 开立医生
	MYSQL_SYNTHESIZE(string, authored_by, Authored_by);
	// 皮试标志
	MYSQL_SYNTHESIZE(string, skin_text, Skin_text);

public:
	EnterOrdersQueryDO() : BaseDO("med_medication_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
		MYSQL_ADD_FIELD("group_id", "s", group_id);
		MYSQL_ADD_FIELD("status", "s", status);
		MYSQL_ADD_FIELD("medicine_type", "s", medicine_type);
		MYSQL_ADD_FIELD("therapy_type", "s", therapy_type);
		MYSQL_ADD_FIELD("medicine_name", "s", medicine_name);
		MYSQL_ADD_FIELD("specification", "s", specification);
		MYSQL_ADD_FIELD("dose", "s", dose);
		MYSQL_ADD_FIELD("dose_unit", "s", dose_unit);
		MYSQL_ADD_FIELD("method", "s", method);
		MYSQL_ADD_FIELD("rate", "s", rate);
		MYSQL_ADD_FIELD("total_price", "s", total_price);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("authored_by", "s", authored_by);
		MYSQL_ADD_FIELD("skin_text", "s", skin_text);
	}
};

// 给MedicalRecordByTitleDO智能指针设定一个别名方便使用
typedef std::shared_ptr<EnterOrdersQueryDO> PtrEnterOrdersQueryDO;





//新增病历DO类
class EnterOrdersAddDO : public BaseDO
{

	// 医嘱主键 med_medication_request.id
	MYSQL_SYNTHESIZE(string, id, Id);

	// 租户id
	MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);
	// 诊断id
	MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);
	// 患者id
	MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
	// 小组id
	MYSQL_SYNTHESIZE(string, group_id, Group_id);

	// 医嘱状态 数据库列：status_enum
	MYSQL_SYNTHESIZE(string, status, Status_enum);
	// 医嘱类型 数据库列：therapy_enum
	MYSQL_SYNTHESIZE(string, therapy_type, Therapy_enum);

	// 单次剂量
	MYSQL_SYNTHESIZE(string, dose, Dose);
	// 剂量单位 数据库列：dose_unit_code
	MYSQL_SYNTHESIZE(string, dose_unit, Dose_unit_code);
	// 服用方式 数据库列：method_code
	MYSQL_SYNTHESIZE(string, method, Method_code);
	// 服用时间 数据库列：rate_code
	MYSQL_SYNTHESIZE(string, rate, Rate_code);

	// 新增时间
	MYSQL_SYNTHESIZE(string, created_at, Created_at);
	// 新增人
	MYSQL_SYNTHESIZE(string, created_by, Created_by);
	// 皮试标志 数据库列：skin_test_flag
	MYSQL_SYNTHESIZE(string, skin_text, Skin_test_flag);

	// 药品id
	MYSQL_SYNTHESIZE(string, medication_id, Medication_id);
	// 诊断id
	MYSQL_SYNTHESIZE(string, condition_definition_id, Condition_definition_id);
	// 诊断id
	MYSQL_SYNTHESIZE(string, bus_no, Bus_no);
	// 诊断id
	MYSQL_SYNTHESIZE(string, quantity, Quantity);
	// 诊断id
	MYSQL_SYNTHESIZE(string, unit_code, Unit_code);
	// 新增时间
	MYSQL_SYNTHESIZE(string, updated_at, Updated_at);
	// 新增人
	MYSQL_SYNTHESIZE(string, updated_by, Updated_by);



public:
	EnterOrdersAddDO() : BaseDO("med_medication_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);

		MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
		MYSQL_ADD_FIELD("group_id", "s", group_id);

		// 关键修正库字段名
		MYSQL_ADD_FIELD("status_enum", "s", status);
		MYSQL_ADD_FIELD("therapy_enum", "s", therapy_type);

		MYSQL_ADD_FIELD("dose", "s", dose);
		MYSQL_ADD_FIELD("dose_unit_code", "s", dose_unit);
		MYSQL_ADD_FIELD("method_code", "s", method);
		MYSQL_ADD_FIELD("rate_code", "s", rate);

		MYSQL_ADD_FIELD("created_at", "s", created_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);

		MYSQL_ADD_FIELD("skin_test_flag", "s", skin_text);

		MYSQL_ADD_FIELD("medication_id", "s", medication_id);
		MYSQL_ADD_FIELD("condition_definition_id", "s", condition_definition_id);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no); 
		MYSQL_ADD_FIELD("quantity", "s", quantity);
		MYSQL_ADD_FIELD("unit_code", "s", unit_code);
		MYSQL_ADD_FIELD("updated_at", "s", updated_at);
		MYSQL_ADD_FIELD("updated_by", "s", updated_by);
	}
};


//新增病历DO类1
class EnterOrdersAddDO1 : public BaseDO
{


	// 账单主键 adm_charge_item.id 映射库字段 id1
	MYSQL_SYNTHESIZE(string, id, Id);
	// 总金额 adm_charge_item.total_price
	MYSQL_SYNTHESIZE(string, service_id, Service_id);
	// 总金额 adm_charge_item.total_price
	MYSQL_SYNTHESIZE(string, total_price, Total_price);
	// 新增时间
	MYSQL_SYNTHESIZE(string, created_at, Created_at);
	// 患者id
	MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
	// 诊断id
	MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);



public:
	EnterOrdersAddDO1() : BaseDO("adm_charge_item")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("service_id", "s", service_id);
		MYSQL_ADD_FIELD("total_price", "s", total_price);
		MYSQL_ADD_FIELD("created_at", "s", created_at);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
	}
};





// 给MedicalRecordAddDO智能指针设定一个别名方便使用
typedef std::shared_ptr<EnterOrdersAddDO> PtrEnterOrdersAddDO;
#endif // !
