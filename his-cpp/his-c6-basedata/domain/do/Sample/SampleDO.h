#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:52:32

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
#ifndef _SAMPLE_DO_
#define _SAMPLE_DO_

#include "../DoInclude.h"

class SampleDO : public BaseDO
{
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);
	MYSQL_SYNTHESIZE(int, version, Version);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(string, remark, Remark);

public:
	SampleDO() : BaseDO("adm_specimen_definition")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("tenant_id", "s", tenantId);
		MYSQL_ADD_FIELD_NULLABLE("created_at", "dt", createdAt, false);
		MYSQL_ADD_FIELD("updated_at", "dt", updatedAt);
		MYSQL_ADD_FIELD("created_by", "s", createdBy);
		MYSQL_ADD_FIELD("updated_by", "s", updatedBy);
		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);
		MYSQL_ADD_FIELD("deleted_at", "dt", deletedAt);
		MYSQL_ADD_FIELD("deleted_by", "s", deletedBy);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, false);
		MYSQL_ADD_FIELD_NULLABLE("name", "s", name, false);
		MYSQL_ADD_FIELD("py_str", "s", pyStr);
		MYSQL_ADD_FIELD("wb_str", "s", wbStr);
		MYSQL_ADD_FIELD_NULLABLE("type_enum", "s", typeEnum, false);
		MYSQL_ADD_FIELD_NULLABLE("status_enum", "s", statusEnum, false);
	}

public:
	string getSampleCode() const { return busNo ? *busNo : string(); }
	void setSampleCode(const string& value) { setBusNo(value); }

	string getSampleName() const { return name ? *name : string(); }
	void setSampleName(const string& value) { setName(value); }

	string getInputCode() const { return pyStr ? *pyStr : string(); }
	void setInputCode(const string& value) { setPyStr(value); }

	string getWbCode() const { return wbStr ? *wbStr : string(); }
	void setWbCode(const string& value) { setWbStr(value); }

	int getStatus() const
	{
		if (!statusEnum)
		{
			return 0;
		}

		const auto& value = *statusEnum;
		return value == "ACTIVE" || value == "PUBLISHED" || value == "ENABLED" ? 1 : 0;
	}

	void setStatus(int value)
	{
		setStatusEnum(value == 1 ? "ACTIVE" : "INACTIVE");
	}

	string getCreateBy() const { return createdBy ? *createdBy : string(); }
	void setCreateBy(const string& value) { setCreatedBy(value); }

	string getCreateTime() const { return createdAt ? *createdAt : string(); }
	void setCreateTime(const string& value) { setCreatedAt(value); }

	string getUpdateBy() const { return updatedBy ? *updatedBy : string(); }
	void setUpdateBy(const string& value) { setUpdatedBy(value); }

	string getUpdateTime() const { return updatedAt ? *updatedAt : string(); }
	void setUpdateTime(const string& value) { setUpdatedAt(value); }
};

typedef std::shared_ptr<SampleDO> PtrSampleDO;

#endif // !_SAMPLE_DO_
