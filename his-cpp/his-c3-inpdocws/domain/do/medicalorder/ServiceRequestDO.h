#pragma once

#include "../DoInclude.h"

class ServiceRequestDO :public BaseDO {
	//主键
	MYSQL_SYNTHESIZE(string, id, Id);
	//状态：DRAFT/ACTIVE/STOPPED/CANCELLED
	MYSQL_SYNTHESIZE(string, status_enum, StatusEnum);
	//医嘱类型：LONG_TERM/TEMPORARY/DISCHARGE_MEDICATIONYS
	MYSQL_SYNTHESIZE(string, therapy_enum, TherapyEnum);

public:
	ServiceRequestDO() :BaseDO("wkf_service_request") {
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("therapy_enum", "s", therapy_enum);
	}
};

// 给ServiceRequestDO智能指针设定一个别名方便使用
typedef std::shared_ptr<ServiceRequestDO> PtrServiceRequestDO;