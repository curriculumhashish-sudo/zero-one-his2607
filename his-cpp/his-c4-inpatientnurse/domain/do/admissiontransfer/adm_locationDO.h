#pragma once

#ifndef _BED_DO_
#define _BED_DO_
#include "../DoInclude.h"

class adm_locationDO : public BaseDO 
{
	//床位id
	MYSQL_SYNTHESIZE(string, id, Id);
	//上级病区id
	MYSQL_SYNTHESIZE(string, parent_id, Parent_id);
	//区分类型
	MYSQL_SYNTHESIZE(string, form_enum, Form_enum);
	//床位状态
	MYSQL_SYNTHESIZE(string, status_enum, Status_enum);
	//床位名称（如301-1床）
	MYSQL_SYNTHESIZE(string, name, Name);
	//所属科室id
	MYSQL_SYNTHESIZE(string, organization_id, Organization_id);

public:
	adm_locationDO() : BaseDO("adm_location") 
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("parent_id", "s", parent_id);
		MYSQL_ADD_FIELD("form_enum", "s", form_enum);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("organization_id", "s", organization_id);
	}

};

typedef std::shared_ptr<adm_locationDO> PtrBedDO;
#endif