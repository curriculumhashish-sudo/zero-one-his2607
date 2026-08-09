#pragma once
#ifndef _CLNCONDITIONDEFINITION_H_
#define _CLNCONDITIONDEFINITION_H_

#include "../DoInclude.h"

class ClnConditionDefinitionDO : public BaseDO
{
public:
	MYSQL_SYNTHESIZE(std::string, id, Id);
	MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(std::string, sourceEnum, SourceEnum);
	MYSQL_SYNTHESIZE(std::string, conditionCode, ConditionCode);
	MYSQL_SYNTHESIZE(std::string, name, Name);
	MYSQL_SYNTHESIZE(std::string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(std::string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(std::string, typeCode, TypeCode);
	MYSQL_SYNTHESIZE(std::string, description, Description);
	MYSQL_SYNTHESIZE(int, ybFlag, YbFlag);
	MYSQL_SYNTHESIZE(std::string, diseaseCode, DiseaseCode);
	MYSQL_SYNTHESIZE(std::string, ybNo, YbNo);
	MYSQL_SYNTHESIZE(int, ybMatchFlag, YbMatchFlag);
	MYSQL_SYNTHESIZE(std::string, statusEnum, StatusEnum);

public:
	ClnConditionDefinitionDO() : BaseDO("cln_condition_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("source_enum", "s", sourceEnum);
		MYSQL_ADD_FIELD("condition_code", "s", conditionCode);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("py_str", "s", pyStr);
		MYSQL_ADD_FIELD("wb_str", "s", wbStr);
		MYSQL_ADD_FIELD("type_code", "s", typeCode);
		MYSQL_ADD_FIELD("description", "s", description);
		MYSQL_ADD_FIELD("yb_flag", "i", ybFlag);
		MYSQL_ADD_FIELD("disease_code", "s", diseaseCode);
		MYSQL_ADD_FIELD("yb_no", "s", ybNo);
		MYSQL_ADD_FIELD("yb_match_flag", "i", ybMatchFlag);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
	}
};

typedef std::shared_ptr<ClnConditionDefinitionDO> PtrClnConditionDefinitionDO;


#endif // !_CLNCONDITIONDEFINITION_H_