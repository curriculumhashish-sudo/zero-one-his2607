#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/27 00:00:00

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
#ifndef _PRACTITIONERDO_H_
#define _PRACTITIONERDO_H_

#include "../DoInclude.h"

// Complete mapping for database table adm_practitioner.
class PractitionerDO : public BaseDO
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
	MYSQL_SYNTHESIZE(string, version, Version);
	MYSQL_SYNTHESIZE(string, name, Name);
	MYSQL_SYNTHESIZE(string, nameJson, NameJson);
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);
	MYSQL_SYNTHESIZE(string, genderEnum, GenderEnum);
	MYSQL_SYNTHESIZE(string, birthDate, BirthDate);
	MYSQL_SYNTHESIZE(string, deceasedDate, DeceasedDate);
	MYSQL_SYNTHESIZE(string, phone, Phone);
	MYSQL_SYNTHESIZE(string, addressProvince, AddressProvince);
	MYSQL_SYNTHESIZE(string, addressCity, AddressCity);
	MYSQL_SYNTHESIZE(string, addressDistrict, AddressDistrict);
	MYSQL_SYNTHESIZE(string, addressStreet, AddressStreet);
	MYSQL_SYNTHESIZE(string, addressJson, AddressJson);
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
	MYSQL_SYNTHESIZE(bool, activeFlag, ActiveFlag);
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	MYSQL_SYNTHESIZE(string, ybNo, YbNo);
	MYSQL_SYNTHESIZE(string, userId, UserId);
	MYSQL_SYNTHESIZE(string, orgId, OrgId);
	MYSQL_SYNTHESIZE(int, drProfttlCode, DrProfttlCode);
	MYSQL_SYNTHESIZE(string, pharPracCertNo, PharPracCertNo);
	MYSQL_SYNTHESIZE(string, prscDrCertCode, PrscDrCertCode);
	MYSQL_SYNTHESIZE(string, signature, Signature);
	MYSQL_SYNTHESIZE(string, kpdCode, KpdCode);
	MYSQL_SYNTHESIZE(string, posNo, PosNo);

public:
	PractitionerDO() : BaseDO("adm_practitioner")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
		MYSQL_ADD_FIELD("is_deleted", "b", isDeleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
		MYSQL_ADD_FIELD("version", "s", version);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD_NULLABLE("name_json", "s", nameJson, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);
		MYSQL_ADD_FIELD("gender_enum", "s", genderEnum);
		MYSQL_ADD_FIELD_NULLABLE("birth_date", "s", birthDate, true);
		MYSQL_ADD_FIELD_NULLABLE("deceased_date", "dt", deceasedDate, true);
		MYSQL_ADD_FIELD_NULLABLE("phone", "s", phone, true);
		MYSQL_ADD_FIELD_NULLABLE("address_province", "s", addressProvince, true);
		MYSQL_ADD_FIELD_NULLABLE("address_city", "s", addressCity, true);
		MYSQL_ADD_FIELD_NULLABLE("address_district", "s", addressDistrict, true);
		MYSQL_ADD_FIELD_NULLABLE("address_street", "s", addressStreet, true);
		MYSQL_ADD_FIELD_NULLABLE("address_json", "s", addressJson, true);
		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
		MYSQL_ADD_FIELD("active_flag", "b", activeFlag);
		MYSQL_ADD_FIELD_NULLABLE("bus_no", "s", busNo, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", ybNo, true);
		MYSQL_ADD_FIELD("user_id", "s", userId);
		MYSQL_ADD_FIELD_NULLABLE("org_id", "s", orgId, true);
		MYSQL_ADD_FIELD_NULLABLE("dr_profttl_code", "i", drProfttlCode, true);
		MYSQL_ADD_FIELD_NULLABLE("phar_prac_cert_no", "s", pharPracCertNo, true);
		MYSQL_ADD_FIELD_NULLABLE("prsc_dr_cert_code", "s", prscDrCertCode, true);
		MYSQL_ADD_FIELD_NULLABLE("signature", "s", signature, true);
		MYSQL_ADD_FIELD_NULLABLE("kpd_code", "s", kpdCode, true);
		MYSQL_ADD_FIELD_NULLABLE("pos_no", "s", posNo, true);
	}
};

typedef std::shared_ptr<PractitionerDO> PtrPractitionerDO;

#endif // _PRACTITIONERDO_H_
