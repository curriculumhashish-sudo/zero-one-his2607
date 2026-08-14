#pragma once
#ifndef _MEDICALRECORDDO_H_
#define _MEDICALRECORDDO_H_
#include "../DoInclude.h"

//获取历史病历列表（条件+分页）DO类
class MedicalRecordByTitleDO : public BaseDO
{
	// 病历主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 病历类型名称
	MYSQL_SYNTHESIZE(string, document_type_name, Document_type_name);
	// 病历名称
	MYSQL_SYNTHESIZE(string, title, Title);
	// 病历状态
	MYSQL_SYNTHESIZE(string, status, Status);
	// 病历更新人id
	MYSQL_SYNTHESIZE(string, updated_at, Updated_at);
public:
	MedicalRecordByTitleDO() : BaseDO("emr_document")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("document_type_name", "s", document_type_name);
		MYSQL_ADD_FIELD("title", "s", title);
		MYSQL_ADD_FIELD("status", "s", status);
		MYSQL_ADD_FIELD("updated_at", "s", updated_at);
	}
};

// 给MedicalRecordByTitleDO智能指针设定一个别名方便使用
typedef std::shared_ptr<MedicalRecordByTitleDO> PtrMedicalRecordByTitleDO;



//获取历史病历详情DO类
class MedicalRecordByIdDO : public BaseDO
{
	// 病历主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 病历类型名称
	MYSQL_SYNTHESIZE(string, content, Content);
public:
	MedicalRecordByIdDO() : BaseDO("emr_document")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("content", "s", content);
	}
};

// 给MedicalRecordByIdDO智能指针设定一个别名方便使用
typedef std::shared_ptr<MedicalRecordByIdDO> PtrMedicalRecordByIdDO;



//新增病历DO类
class MedicalRecordAddDO : public BaseDO
{
	//病历主键
	MYSQL_SYNTHESIZE(string, id, Id);
	//病人id
	MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
	//门诊id
	MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);
	//病历类型编码
	MYSQL_SYNTHESIZE(string, document_type_code, Document_type_code);
	////病历类型名称
	//MYSQL_SYNTHESIZE(string, document_type_name, Document_type_name);
	//病历名称
	MYSQL_SYNTHESIZE(string, title, Title);
	//病历状态
	MYSQL_SYNTHESIZE(string, status, Status);
	//病历内容详情
	MYSQL_SYNTHESIZE(string, content, Content);
	//病历创建时间
	MYSQL_SYNTHESIZE(string, created_at, Created_at);
	////病历更新时间
	//MYSQL_SYNTHESIZE(string, updated_at, Updated_at);
	//病历创建人id
	MYSQL_SYNTHESIZE(string, created_by, Created_by);
	////病历更新人id
	//MYSQL_SYNTHESIZE(string, updated_by, Updated_by);
public:
	MedicalRecordAddDO() : BaseDO("emr_document")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("patient_id", "s", patient_id);
		MYSQL_ADD_FIELD("encounter_id", "s", encounter_id);
		MYSQL_ADD_FIELD("document_type_code", "s", document_type_code);
		//MYSQL_ADD_FIELD("document_type_name", "s", document_type_name);
		MYSQL_ADD_FIELD("title", "s", title);
		MYSQL_ADD_FIELD("status", "s", status);
		MYSQL_ADD_FIELD("content", "s", content);
		MYSQL_ADD_FIELD("created_at", "s", created_at);
		//MYSQL_ADD_FIELD("updated_at", "s", updated_at);
		MYSQL_ADD_FIELD("created_by", "s", created_by);
		//MYSQL_ADD_FIELD("updated_by", "s", updated_by);
	}
};

// 给MedicalRecordAddDO智能指针设定一个别名方便使用
typedef std::shared_ptr<MedicalRecordAddDO> PtrMedicalRecordAddDO;
#endif // !
