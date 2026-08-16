/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/08/08 10:00:00

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
#include "stdafx.h"
#include "InpatientAdmissionService.h"
#include "BaseDAO.h"
#include "Mapper.h"

namespace {
	struct ScalarStr { std::string val; };
	class ScalarStrMapper : public Mapper<ScalarStr>
	{
	public:
		ScalarStr mapper(ResultSet* rs) const override
		{
			ScalarStr s; s.val = rs->getString(1); return s;
		}
	};

	struct WardRow { std::string id; std::string name; };
	class WardRowMapper : public Mapper<WardRow>
	{
	public:
		WardRow mapper(ResultSet* rs) const override
		{
			WardRow row;
			row.id = rs->getString(1);
			row.name = rs->getString(2);
			return row;
		}
	};
}

std::list<InpatientAdmissionWardVO::Wrapper> InpatientAdmissionService::listWards(
	const std::string& departmentId, bool& deptValid)
{
	std::list<InpatientAdmissionWardVO::Wrapper> wards;
	deptValid = false;

	BaseDAO dao;
	auto* ss = dao.getSqlSession();

	// Validate the target department: it must exist, be enabled, be an
	// inpatient (non-outpatient) department, and not be soft deleted.
	std::string validId = ss->executeQueryOne<ScalarStr>(
		"SELECT id FROM adm_organization "
		"WHERE id = ? AND type_enum = 'DEPARTMENT' AND class_enum = 'INPATIENT' "
		"AND status_enum = 'ACTIVE' AND active_flag = 1 AND is_deleted = 0 LIMIT 1",
		ScalarStrMapper(), "%s", departmentId).val;
	if (validId.empty()) return wards;
	deptValid = true;

	// Load the enabled wards owned by the target department.
	// A ward is a row of adm_location whose form_enum is 'WARD'.
	auto rows = ss->executeQuery<WardRow>(
		"SELECT id, name FROM adm_location "
		"WHERE organization_id = ? AND form_enum = 'WARD' "
		"AND status_enum = 'ACTIVE' AND is_deleted = 0 "
		"ORDER BY display_order",
		WardRowMapper(), "%s", departmentId);

	for (const auto& row : rows)
	{
		auto vo = InpatientAdmissionWardVO::createShared();
		vo->value = row.id;
		vo->label = row.name;
		vo->departmentId = departmentId;
		wards.push_back(vo);
	}
	return wards;
}