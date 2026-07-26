#pragma once
#ifndef _ENCOUNTER_DO_
#define _ENCOUNTER_DO_

#include "../DoInclude.h"

class EncounterDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(bool, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, patientId, PatientId);
	MYSQL_SYNTHESIZE(string, groupId, GroupId);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, classEnum, ClassEnum);
	MYSQL_SYNTHESIZE(string, ybClassEnum, YbClassEnum);
	MYSQL_SYNTHESIZE(string, ybClassText, YbClassText);
	MYSQL_SYNTHESIZE(string, classJson, ClassJson);
	MYSQL_SYNTHESIZE(string, priorityEnum, PriorityEnum);
	MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
	MYSQL_SYNTHESIZE(string, serviceTypeId, ServiceTypeId);
	MYSQL_SYNTHESIZE(string, subjectStatusEnum, SubjectStatusEnum);
	MYSQL_SYNTHESIZE(string, startTime, StartTime);
	MYSQL_SYNTHESIZE(string, endTime, EndTime);
	MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);
	MYSQL_SYNTHESIZE(int, displayOrder, DisplayOrder);
	MYSQL_SYNTHESIZE(string, firstEnum, FirstEnum);
	MYSQL_SYNTHESIZE(string, admitSourceCode, AdmitSourceCode);
	MYSQL_SYNTHESIZE(string, inWayCode, InWayCode);
	MYSQL_SYNTHESIZE(string, receptionTime, ReceptionTime);
	MYSQL_SYNTHESIZE(string, registrarId, RegistrarId);
	MYSQL_SYNTHESIZE(string, ambEncounterId, AmbEncounterId);
	MYSQL_SYNTHESIZE(string, outWayCode, OutWayCode);

public:
	EncounterDO() : BaseDO("adm_encounter")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD("created_at", "s", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("patient_id", "s", patientId);
		MYSQL_ADD_FIELD_NULLABLE("group_id", "s", groupId);
		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("class_enum", "s", classEnum);
		MYSQL_ADD_FIELD_NULLABLE("yb_class_enum", "s", ybClassEnum);
		MYSQL_ADD_FIELD_NULLABLE("yb_class_text", "s", ybClassText);
		MYSQL_ADD_FIELD_NULLABLE("class_json", "s", classJson);
		MYSQL_ADD_FIELD_NULLABLE("priority_enum", "s", priorityEnum);
		MYSQL_ADD_FIELD_NULLABLE("type_enum", "s", typeEnum);
		MYSQL_ADD_FIELD_NULLABLE("service_type_id", "s", serviceTypeId);
		MYSQL_ADD_FIELD_NULLABLE("subject_status_enum", "s", subjectStatusEnum);
		MYSQL_ADD_FIELD_NULLABLE("start_time", "s", startTime);
		MYSQL_ADD_FIELD_NULLABLE("end_time", "s", endTime);
		MYSQL_ADD_FIELD("organization_id", "s", organizationId);
		MYSQL_ADD_FIELD_NULLABLE("display_order", "i", displayOrder);
		MYSQL_ADD_FIELD_NULLABLE("first_enum", "s", firstEnum);
		MYSQL_ADD_FIELD_NULLABLE("admit_source_code", "s", admitSourceCode);
		MYSQL_ADD_FIELD_NULLABLE("in_way_code", "s", inWayCode);
		MYSQL_ADD_FIELD_NULLABLE("reception_time", "s", receptionTime);
		MYSQL_ADD_FIELD_NULLABLE("registrar_id", "s", registrarId);
		MYSQL_ADD_FIELD_NULLABLE("amb_encounter_id", "s", ambEncounterId);
		MYSQL_ADD_FIELD_NULLABLE("out_way_code", "s", outWayCode);
	}
};

typedef std::shared_ptr<EncounterDO> PtrEncounterDO;

#endif
