#pragma once
#ifndef _DIAGNOSISENTRYDO_H_
#define _DIAGNOSISENTRYDO_H_
#include "../DoInclude.h"
//DiagnosisEntry
//diagnosis-entry
//DIAGNOSISENTRY
// 
//获取获取诊断使用历史DTO类
class DiagnosisEntryBriefDO : public BaseDO
{
	// 诊断主键
	MYSQL_SYNTHESIZE(string, condition_id, Condition_id);
	// 诊断症状名称
	MYSQL_SYNTHESIZE(string, definition_name, Definition_name);
	DiagnosisEntryBriefDO() : BaseDO("cln_condition")
	{
		MYSQL_ADD_FIELD_PK("condition_id", "s", condition_id);
		MYSQL_ADD_FIELD("definition_name", "s", definition_name);
	}
};

// 给MedicalRecordByTitleDO智能指针设定一个别名方便使用
typedef std::shared_ptr<DiagnosisEntryBriefDO> PtrDiagnosisEntryBriefDO;



//获取患者诊断记录（条件+分页）DTO类
class DiagnosisEntryDetailDO : public BaseDO
{
	// 诊断主键
	MYSQL_SYNTHESIZE(string, condition_id, Condition_id);
	// 诊断症状编码
	MYSQL_SYNTHESIZE(string, definition_code, Definition_code);
	// 诊断描述
	MYSQL_SYNTHESIZE(string, description, Description);
	// 医学类型名称
	MYSQL_SYNTHESIZE(string, kind_name, Kind_name);
	// 诊断类型名称
	MYSQL_SYNTHESIZE(string, type_name, Type_name);
	// 主诊标注
	MYSQL_SYNTHESIZE(string, main_flag, Main_flag);
	// 主诊标注
	MYSQL_SYNTHESIZE(string, adm_encounter_diagnosis_id, Adm_encounter_diagnosis_id);
public:
	DiagnosisEntryDetailDO() : BaseDO("cln_condition")
	{
		MYSQL_ADD_FIELD_PK("condition_id", "s", condition_id);
		MYSQL_ADD_FIELD("definition_code", "s", definition_code);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("kind_name", "s", kind_name);
		MYSQL_ADD_FIELD("type_name", "s", type_name);
		MYSQL_ADD_FIELD("main_flag", "s", main_flag);
		MYSQL_ADD_FIELD("adm_encounter_diagnosis_id", "s", adm_encounter_diagnosis_id);
	}
};

// 给MedicalRecordByIdDO智能指针设定一个别名方便使用
typedef std::shared_ptr<DiagnosisEntryDetailDO> PtrDiagnosisEntryDetailDO;



#endif // !
