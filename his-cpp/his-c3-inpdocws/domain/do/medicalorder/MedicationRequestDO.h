#pragma once 

#include "../DoInclude.h"

/**
 * 数据库实体类
 */
class MedicationRequestDO : public BaseDO
{
	//主键
	MYSQL_SYNTHESIZE(string, id, Id);
	//状态：DRAFT/ACTIVE/STOPPED/CANCELLED
	MYSQL_SYNTHESIZE(string, status_enum, StatusEnum);
	//医嘱类型：LONG_TERM/TEMPORARY/DISCHARGE_MEDICATIONYS
	MYSQL_SYNTHESIZE(string, therapy_enum, TherapyEnum);

	
public:
	MedicationRequestDO() : BaseDO("med_medication_request")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("therapy_enum", "s", therapy_enum);
	}
};

// 给MedicationRequestDO智能指针设定一个别名方便使用
typedef std::shared_ptr<MedicationRequestDO> PtrMedicationRequestDO;